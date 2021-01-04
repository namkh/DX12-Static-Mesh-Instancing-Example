#include "StaticMeshInstancingExample.h"
#include "DX12DeviceResourceAccess.h"
#include "RenderObjectContainer.h"
#include "GeometryContainer.h"
#include "TextureContainer.h"
#include "MaterialContainer.h"
#include "GlobalSystemValues.h"
#include "Utils.h"


bool StaticMeshInstancingExample::Initialize()
{
	m_camera.Initialize
	(
		static_cast<float>(m_width), 
		static_cast<float>(m_height),
		GlobalSystemValues::Instance().ViewportNearDistance, 
		GlobalSystemValues::Instance().ViewportFarDistance,
		GlobalSystemValues::Instance().FovAngleY
	);
	m_camera.SetRadius(200.0f);
	m_camera.SetSeta(PI * -0.5f);
	m_camera.SetPhi(PI * 0.5f);
	m_camera.SetZoomSensitivity(10.0f);

	gFbxGeomLoader.Initialize();

	m_globalConstantsBuffer.Initialize(1, true);
	
	m_fences.resize(FRAME_COUNT);
	for (auto& cur : m_fences)
	{
		cur.Initialize(L"MainFence");
	}

	m_updateCommands.Initialize(FRAME_COUNT);

	if (!CreateRenderObjects())
	{
		return false;
	}
	m_screenSizeChangedCallbackHandle = gDX12DeviceRes->OnRenderTargetSizeChanged.Add
	(
		[this]()
		{
			OnScreenSizeChanged(gDX12DeviceRes->GetWidth(), gDX12DeviceRes->GetHeight());
		}
	);
	
	m_keyEventHandle = CoreEventManager::Instance().RegisterKeyboardEventCallback(this, &StaticMeshInstancingExample::OnKeyboardEvent);

	return true;
}

void StaticMeshInstancingExample::Update(float timeDelta)
{
	
}

void StaticMeshInstancingExample::PreRender()
{
	m_fences[m_currentFrame].WaitForEnd();

	m_globalConstants.MatViewProj = XMMatrixTranspose(m_camera.GetViewProjectionMatrix());
	m_globalConstants.CamPos = m_camera.GetPosition();

	CommandBuffer* cmdBuf = m_updateCommands.GetCommandBuffer(m_currentFrame);
	cmdBuf->Begin(nullptr);
	m_globalConstantsBuffer.UploadResource(cmdBuf, reinterpret_cast<void*>(&m_globalConstants));
	cmdBuf->End();
}

void StaticMeshInstancingExample::Render()
{
	std::vector<CommandBufferBase*> submitCommands;
	for (auto& cur : m_updateCommandLists)
	{
		submitCommands.push_back(cur);
	}
	
	auto iterRenderItemFinded = m_renderItems.find(m_currentRenderItem);
	if (iterRenderItemFinded != m_renderItems.end())
	{
		if (iterRenderItemFinded->second != nullptr)
		{
			std::vector<CommandBuffer*> updateCommandBuffers;
			CommandBuffer* cmdBuf = m_updateCommands.GetCommandBuffer(m_currentFrame);
			if (cmdBuf != nullptr && cmdBuf->GetState() == CommandBuffer::ECommandBufferState::COMMAND_BUFFER_STATE_RECORDED)
			{
				updateCommandBuffers.push_back(cmdBuf);
			}
			
			iterRenderItemFinded->second->PopUpdateCommandBuffers(updateCommandBuffers);
			for (auto& cur : updateCommandBuffers)
			{
				if (cur != nullptr)
				{
					submitCommands.push_back(cur);
				}
			}

			CommandBuffer* renderItemCmdBuf = iterRenderItemFinded->second->GetRenderCommandBuffer(m_currentFrame);
			if (renderItemCmdBuf != nullptr)
			{
				submitCommands.push_back(renderItemCmdBuf);
			}
		}
	}

	if (submitCommands.size() > 0)
	{
		gDX12DeviceRes->GetDefaultCommandQueue().Submit(submitCommands, &m_fences[m_currentFrame], true);
	}

	m_currentFrame = (m_currentFrame + 1) % FRAME_COUNT;
}

void StaticMeshInstancingExample::Destroy()
{
	CoreEventManager::Instance().UnregisterEventCallback(m_keyEventHandle);
	gDX12DeviceRes->OnRenderTargetSizeChanged.Remove(m_screenSizeChangedCallbackHandle);
	gDX12DeviceRes->GetDefaultCommandQueue().WaitForAllFence();
	
	m_camera.Destroy();
	for (auto& cur : m_renderItems)
	{
		if (cur.second != nullptr)
		{
			cur.second->Destroy();
			delete cur.second;
		}
	}
	m_renderItems.clear();
	for (auto& cur : m_fences)
	{
		cur.Destory();
	}

	m_updateCommands.Destory();

	m_globalConstantsBuffer.Destroy();

	gRenderObjContainer.Clear();
	gGeomContainer.Clear();
	gMaterialContainer.Clear();
	gTexContainer.Clear();
	gCmdAllocMgr.Destory();
}

void StaticMeshInstancingExample::OnKeyboardEvent(KeyboardEvent* keyEvent)
{
	if (keyEvent->m_keyType == EKeyboaradEvent::SPACE)
	{
		if (m_currentRenderItem == ERenderItemType::MESH_SHADER_INSTANCING)
		{
			m_appName = L"Hardware Instancing";
			m_currentRenderItem = ERenderItemType::HARDWARE_INSTANCING;
		}
		else
		{
			m_appName = L"Mesh Shader Instancing";
			m_currentRenderItem = ERenderItemType::MESH_SHADER_INSTANCING;
		}
	}
}

void StaticMeshInstancingExample::OnScreenSizeChanged(uint32_t width, uint32_t height)
{
	for (auto& cur : m_renderItems)
	{
		if (cur.second != nullptr)
		{
			cur.second->RefreshRenderCommandBuffer();
		}
	}
	m_currentFrame = 0;
}

bool StaticMeshInstancingExample::CreateRenderObjects()
{
	SingleTimeCommandBuffer singleTimeCommandBuffer;
	singleTimeCommandBuffer.Begin();
	SimpleRenderObject* renderObj = gRenderObjContainer.CreateRenderObject(&singleTimeCommandBuffer, "../Resources/Mesh/MeetMat.fbx", ExampleMaterialType::EXAMPLE_MAT_HARDWARE_INSTANCING1, true);

	float startPosX = m_objectArea.x * -0.5f;
	float startPosY = m_objectArea.y * -0.5f;
	float startPosZ = m_objectArea.z * -0.5f;
	
	float intervalX = m_objectArea.x * (1.0f / 24.0f);
	float intervalY = m_objectArea.y * (1.0f / 19.0f);
	float intervalZ = m_objectArea.z * (1.0f / 19.0f);

	int matIndex = 0;
	for (uint32_t i = 0; i < m_objectCountZ; i++)
	{
		for (uint32_t j = 0; j < m_objectCountY; j++)
		{
			for (uint32_t k = 0; k < m_objectCountX; k++)
			{
				XMMATRIX matWorld = XMMatrixTranslation(startPosX + k * intervalX, startPosY + j * intervalY, startPosZ + i * intervalZ);
				SampleRenderObjectInstance* objInst = renderObj->CreateInstance(matWorld);
				switch (matIndex)
				{
					case 0:
						objInst->SetOverrideMaterial(&singleTimeCommandBuffer, ExampleMaterialType::EXAMPLE_MAT_HARDWARE_INSTANCING1);
						break;
					case 1:
						objInst->SetOverrideMaterial(&singleTimeCommandBuffer, ExampleMaterialType::EXAMPLE_MAT_HARDWARE_INSTANCING2);
						break;
					case 2:
						objInst->SetOverrideMaterial(&singleTimeCommandBuffer, ExampleMaterialType::EXAMPLE_MAT_HARDWARE_INSTANCING3);
						break;
				}
				++matIndex;
				if (matIndex >= 3)
				{
					matIndex = 0;
				}
			}
		}
	}
	
	RenderItem_HardwareInstancing* hardwareInstancingRenderItem = new RenderItem_HardwareInstancing();
	hardwareInstancingRenderItem->Initialize
	(
		renderObj, 
		&m_globalConstantsBuffer, 
		FRAME_COUNT
	);
	hardwareInstancingRenderItem->UpdateResources();
	m_renderItems.insert(std::make_pair(ERenderItemType::HARDWARE_INSTANCING, hardwareInstancingRenderItem));

	RenderItem_MeshShaderInstancing* meshShaderInstancingRenderItem = new RenderItem_MeshShaderInstancing();
	meshShaderInstancingRenderItem->Initialize
	(
		renderObj,
		&m_globalConstantsBuffer,
		FRAME_COUNT
	);
	meshShaderInstancingRenderItem->UpdateResources();
	m_renderItems.insert(std::make_pair(ERenderItemType::MESH_SHADER_INSTANCING, meshShaderInstancingRenderItem));

	singleTimeCommandBuffer.End();
	return true;
}