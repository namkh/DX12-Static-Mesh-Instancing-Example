
#include "DeviceBuffers.h"

ReadOnlyDeviceBuffer::ReadOnlyDeviceBuffer()
{

}

ReadOnlyDeviceBuffer::~ReadOnlyDeviceBuffer()
{
}

bool ReadOnlyDeviceBuffer::Initialize(uint32_t count, uint32_t stride)
{
	m_count = count;
	m_stride = stride;
	m_byteSize = m_count * m_stride;

	HRESULT res = gLogicalDevice->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(m_stride * count),
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&m_resourceBuffer)
	);

	if (FAILED(res))
	{
		return false;
	}

	res = gLogicalDevice->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(m_stride * count),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_stagingBuffer)
	);

	if (FAILED(res))
	{
		return false;
	}

	return true;
}

void ReadOnlyDeviceBuffer::UploadResource(CommandBufferBase* cmdBuffer, void* data)
{
	if (m_stagingBuffer != nullptr)
	{
		D3D12_SUBRESOURCE_DATA subResourceData;
		subResourceData.pData = data;
		subResourceData.RowPitch = m_stride;
		subResourceData.SlicePitch = m_stride;

		uint8_t* memory = nullptr;
		if (SUCCEEDED(m_stagingBuffer->Map(0, nullptr, reinterpret_cast<void**>(&memory))))
		{
			std::memcpy(memory, data, m_byteSize);
			m_stagingBuffer->Unmap(0, nullptr);

			cmdBuffer->GetCommandBuffer()->ResourceBarrier
			(
				1,
				&CD3DX12_RESOURCE_BARRIER::Transition
				(
					m_resourceBuffer,
					D3D12_RESOURCE_STATE_COMMON,
					D3D12_RESOURCE_STATE_COPY_DEST
				)
			);

			cmdBuffer->GetCommandBuffer()->CopyResource(m_resourceBuffer, m_stagingBuffer);

			cmdBuffer->GetCommandBuffer()->ResourceBarrier
			(
				1,
				&CD3DX12_RESOURCE_BARRIER::Transition
				(
					m_resourceBuffer,
					D3D12_RESOURCE_STATE_COPY_DEST,
					m_resourceState
				)
			);
		}
		else
		{
			REPORT(EReportType::REPORT_TYPE_WARN, "Staging buffer map failed.");
		}
	}
	else
	{
		REPORT(EReportType::REPORT_TYPE_WARN, "The buffer has already been uploaded.");
	}
}

void ReadOnlyDeviceBuffer::Destroy()
{
	if (m_resourceBuffer != nullptr)
	{
		m_resourceBuffer->Release();
	}
	
	if (m_stagingBuffer != nullptr)
	{
		m_stagingBuffer->Release();
	}
}

VertexBuffer::VertexBuffer()
{
	m_resourceState = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
}

VertexBuffer::~VertexBuffer()
{

}

bool VertexBuffer::Initialize(CommandBufferBase* cmdBuffer, std::vector<DefaultVertex>& vertexList)
{
	if (ReadOnlyDeviceBuffer::Initialize(static_cast<uint32_t>(vertexList.size()), sizeof(DefaultVertex)))
	{
		m_resourceBuffer->SetName(L"vb resource buffer");
		m_stagingBuffer->SetName(L"vb staging buffer");
		UploadResource(cmdBuffer, reinterpret_cast<void*>(vertexList.data()));
		m_inputLayout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 52, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
		return true;
	}
	return false;
}

IndexBuffer::IndexBuffer()
{
	m_resourceState = D3D12_RESOURCE_STATE_INDEX_BUFFER;
}

IndexBuffer::~IndexBuffer()
{

}

bool IndexBuffer::Initialize(CommandBufferBase* cmdBuffer, std::vector<uint32_t>& indexList)
{
	if (ReadOnlyDeviceBuffer::Initialize(static_cast<uint32_t>(indexList.size()), sizeof(uint32_t)))
	{
		m_resourceBuffer->SetName(L"ib resource buffer");
		m_stagingBuffer->SetName(L"ib staging buffer");
		UploadResource(cmdBuffer, reinterpret_cast<void*>(indexList.data()));
		
		return true;
	}
	return false;
}