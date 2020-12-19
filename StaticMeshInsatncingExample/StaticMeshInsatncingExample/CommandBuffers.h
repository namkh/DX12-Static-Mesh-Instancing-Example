#pragma once

#include <vector>

#include "Defaults.h"
#include "Commands.h"
#include "Fence.h"

class CommandBufferBase
{
public:
	virtual ~CommandBufferBase() {}
public:
	void FreeCommandBuffers();

protected:
	bool AllocateCommandBuffer(ID3D12CommandAllocator* cmdPool);
	virtual bool Begin();
	virtual bool Begin(ID3D12PipelineState* pso);
	virtual bool End();

public:
	ID3D12GraphicsCommandList* GetCommandBuffer()
	{
		if (m_isInitialized)
		{
			return m_commandBuffer;
		}
		return nullptr;
	};

protected:

	ID3D12GraphicsCommandList* m_commandBuffer;
	ID3D12CommandAllocator* m_commandPool;
	ID3D12CommandQueue* m_commandQueue;
	bool m_isInitialized = false;
};

class CommandBuffer : public CommandBufferBase
{
public:
	virtual ~CommandBuffer() {}
public:
	enum class ECommandBufferState
	{
		COMMAND_BUFFER_STATE_READY,
		COMMAND_BUFFER_STATE_RECORDED,
		COMMAND_BUFFER_STATE_SUBMITTED,
	};
public:
	bool Initialize(ID3D12CommandAllocator* cmdPool);
	virtual bool Begin(ID3D12PipelineState* pso) override;
	virtual bool End() override;

	ECommandBufferState GetState() { return m_state; }
	void OnSubmitted() { m_state = ECommandBufferState::COMMAND_BUFFER_STATE_SUBMITTED; }
	void OnExecutionEnded() { m_state = ECommandBufferState::COMMAND_BUFFER_STATE_READY; }

private:

	ECommandBufferState m_state = ECommandBufferState::COMMAND_BUFFER_STATE_READY;
};

class StaticCommandBufferContainer
{
public:
	bool Initialize(uint32_t allocCount);
	void Destory();
	CommandBuffer* GetCommandBuffer(uint32_t index);
	void Clear();

	uint32_t GetCommandBufferCount() { return static_cast<uint32_t>(m_commandList.size()); }

private:
	std::vector<CommandBuffer> m_commandList;
	ID3D12CommandAllocator*	m_commandAllocator;
};

class DynamicCommandBufferContainer
{
public:
	bool Initialize();
	void Destory();
	CommandBuffer* GetCommandBuffer();
	CommandBuffer* GetCommandBuffer(uint32_t index);
	void Clear();

	uint32_t GetCommandBufferCount() { return static_cast<uint32_t>(m_commandList.size()); }

private:

	std::vector<CommandBuffer> m_commandList;
	ID3D12CommandAllocator*	m_commandAllocator;
};

class SingleTimeCommandBuffer : public CommandBufferBase
{
public:
	virtual ~SingleTimeCommandBuffer() {}
public:
	virtual bool Begin() override;
	virtual bool Begin(ID3D12PipelineState* pso) override;
	virtual bool End() override;

private:
	Fence m_fence = {};
	ID3D12CommandAllocator*	m_commandAllocator;
};