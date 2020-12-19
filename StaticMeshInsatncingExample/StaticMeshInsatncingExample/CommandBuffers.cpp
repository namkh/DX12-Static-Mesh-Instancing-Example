#include "CommandBuffers.h"
#include "DX12DeviceResourceAccess.h"	
#include "Fence.h"

void CommandBufferBase::FreeCommandBuffers()
{
	if (m_isInitialized)
	{
		m_commandBuffer->Release();
	}
}

bool CommandBufferBase::AllocateCommandBuffer(ID3D12CommandAllocator* cmdPool)
{
	m_commandPool = cmdPool;

	HRESULT res = gLogicalDevice->CreateCommandList
	(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_commandPool,
		nullptr,
		IID_PPV_ARGS(&m_commandBuffer)
	);
	m_commandBuffer->Close();

	return SUCCEEDED(res);
}

bool CommandBufferBase::Begin()
{
	return SUCCEEDED(m_commandBuffer->Reset(m_commandPool, nullptr));
}

bool CommandBufferBase::Begin(ID3D12PipelineState* pso)
{
	return SUCCEEDED(m_commandBuffer->Reset(m_commandPool, pso));
}

bool CommandBufferBase::End()
{
	return SUCCEEDED(m_commandBuffer->Close());
}

bool CommandBuffer::Initialize(ID3D12CommandAllocator* cmdPool)
{
	if (AllocateCommandBuffer(cmdPool))
	{
		m_isInitialized = true;
		return true;
	}
	return false;
}

bool CommandBuffer::Begin(ID3D12PipelineState* pso)
{
	return CommandBufferBase::Begin(pso);
}

bool CommandBuffer::End()
{
	m_state = ECommandBufferState::COMMAND_BUFFER_STATE_RECORDED;
	return CommandBufferBase::End();
}

bool StaticCommandBufferContainer::Initialize(uint32_t allocCount)
{
	HRESULT res = gLogicalDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator));
	if (FAILED(res))
	{
		REPORT(EReportType::REPORT_TYPE_ERROR, "Command allocator create failed.");
		return false;
	}
	m_commandAllocator->SetName(L"Static Command Buffer Container Allocator");

	m_commandList.resize(allocCount);
	for (auto& cur : m_commandList)
	{
		cur.Initialize(m_commandAllocator);
	}

	return true;
}

void StaticCommandBufferContainer::Destory()
{
	Clear();

	if (m_commandAllocator != nullptr)
	{
		m_commandAllocator->Release();
	}
}

CommandBuffer* StaticCommandBufferContainer::GetCommandBuffer(uint32_t index)
{
	if (index < m_commandList.size())
	{
		return &m_commandList[index];
	}
	return nullptr;
}

void StaticCommandBufferContainer::Clear()
{
	for (auto& cur : m_commandList)
	{
		cur.FreeCommandBuffers();
	}
	m_commandList.clear();
}

bool DynamicCommandBufferContainer::Initialize()
{
	HRESULT res = gLogicalDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator));
	if (FAILED(res))
	{
		REPORT(EReportType::REPORT_TYPE_ERROR, "Command allocator create failed.");
		return false;
	}
	m_commandAllocator->Reset();

	return true;
}

void DynamicCommandBufferContainer::Destory()
{
	Clear();

	if (m_commandAllocator != nullptr)
	{
		m_commandAllocator->Release();
	}
}

CommandBuffer* DynamicCommandBufferContainer::GetCommandBuffer()
{
	for (int i = 0; i < m_commandList.size(); i++)
	{
		if (m_commandList[i].GetState() == CommandBuffer::ECommandBufferState::COMMAND_BUFFER_STATE_READY)
		{
			return &m_commandList[i];
		}
	}

	m_commandList.emplace_back();
	m_commandList[m_commandList.size() - 1].Initialize(m_commandAllocator);
	return &m_commandList[m_commandList.size() - 1];

	return nullptr;
}

CommandBuffer* DynamicCommandBufferContainer::GetCommandBuffer(uint32_t index)
{
	if (index < m_commandList.size())
	{
		if (m_commandList[index].GetState() == CommandBuffer::ECommandBufferState::COMMAND_BUFFER_STATE_READY)
		{
			return &m_commandList[index];
		}
		else
		{
			//인덱스에 해당하는 커맨드 버퍼가 준비되지 않았음을 로깅
		}
	}
	return nullptr;
}

void DynamicCommandBufferContainer::Clear()
{
	for (auto& cur : m_commandList)
	{
		cur.FreeCommandBuffers();
	}
	m_commandList.clear();
}

bool SingleTimeCommandBuffer::Begin()
{
	HRESULT res = gLogicalDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator));
	if (FAILED(res))
	{
		REPORT(EReportType::REPORT_TYPE_ERROR, "Command allocator create failed.");
	}
	m_commandAllocator->Reset();
	m_commandAllocator->SetName(L"SingleTimeCmdAllocator");

	m_commandPool = m_commandAllocator;
	if (AllocateCommandBuffer(m_commandPool))
	{
		if (CommandBufferBase::Begin())
		{
			m_isInitialized = true;
		}
		else
		{
			REPORT(EReportType::REPORT_TYPE_ERROR, "Single time command buffer begin failed");
			return false;
		}
	}
	else
	{
		REPORT(EReportType::REPORT_TYPE_ERROR, "Single time command buffer allocate failed");
		return false;
	}

	return true;
}

bool SingleTimeCommandBuffer::Begin(ID3D12PipelineState* pso)
{
	HRESULT res = gLogicalDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator));
	if (FAILED(res))
	{
		REPORT(EReportType::REPORT_TYPE_ERROR, "Command allocator create failed.");
	}
//	m_commandAllocator->Reset();

	m_commandPool = m_commandAllocator;
	if (AllocateCommandBuffer(m_commandPool))
	{
		if (CommandBufferBase::Begin(pso))
		{
			m_isInitialized = true;
		}
		else
		{
			REPORT(EReportType::REPORT_TYPE_ERROR, "Single time command buffer begin failed");
			return false;
		}
	}
	else
	{
		REPORT(EReportType::REPORT_TYPE_ERROR, "Single time command buffer allocate failed");
		return false;
	}

	return true;
}

bool SingleTimeCommandBuffer::End()
{
	if (CommandBufferBase::End())
	{
		m_fence.Initialize(L"SCmdBufFence");
		gDX12DeviceRes->GetDefaultCommandQueue().Submit(this, &m_fence, false);
		m_fence.WaitForEnd();
		m_fence.Destory();
		
		FreeCommandBuffers();

		m_commandPool = nullptr;
		m_commandAllocator->Release();
	}
	else
	{
		REPORT(EReportType::REPORT_TYPE_ERROR, "Single time command buffer end failed");
		return false;
	}
	return true;
}