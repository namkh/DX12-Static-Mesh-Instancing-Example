#include "RenderObjectContainer.h"

SimpleRenderObject* RenderObjectContainer::CreateRenderObject(CommandBufferBase* cmdBuffer, std::string fbxFilePath, ExampleMaterialType exampleMaterialType, bool useMeshShader)
{
	SimpleRenderObject* obj = Create<SimpleRenderObject, std::unordered_map<UID, SimpleRenderObject*> >(&m_renderObjList);
	obj->Initialize(cmdBuffer, fbxFilePath, exampleMaterialType, useMeshShader);
	return obj;
}

void RenderObjectContainer::RemoveRenderObject(SimpleRenderObject* obj)
{
	Remove(obj, &m_renderObjList);
}

SampleRenderObjectInstance* RenderObjectContainer::CreateRenderObjectInstance(XMMATRIX& matWorld)
{
	SampleRenderObjectInstance* inst = Create<SampleRenderObjectInstance, std::unordered_map<UID, SampleRenderObjectInstance*> >(&m_instList);
	inst->Initialzie(matWorld);
	
	OnInstPerMeshAdded.Exec(static_cast<uint32_t>(m_instPerMeshList.size() - 1));

	return inst;
}

void RenderObjectContainer::RemoveRenderObjectInstance(SampleRenderObjectInstance* instance)
{
	Remove(instance, &m_instList);
}

SampleRenderObjectInstancePerMesh* RenderObjectContainer::CreateRenderObjectInstancePerMesh(SampleRenderObjectInstance* parentInst, SimpleMeshData* meshData, SimpleMaterial* material)
{
	SampleRenderObjectInstancePerMesh* instPerMesh = Create<SampleRenderObjectInstancePerMesh, std::unordered_map<UID, SampleRenderObjectInstancePerMesh*> >(&m_instPerMeshList);
	instPerMesh->Initialize(parentInst, meshData, material);
	
	OnInstPerMeshAdded.Exec();

	return instPerMesh;
}

void RenderObjectContainer::RemoveRenderObjectInstancePerMesh(SampleRenderObjectInstancePerMesh* instancePreMesh)
{
	int removeIndex = GetRenderObjectInstancePerMeshBindIndex(instancePreMesh);
	if (removeIndex != -1)
	{
		Remove(instancePreMesh, &m_instPerMeshList);

		OnInstPerMeshRemoved.Exec(static_cast<uint32_t>(removeIndex));
	}
}

void RenderObjectContainer::Clear()
{
	for (auto& cur : m_renderObjList)
	{
		if (cur.second != nullptr)
		{
			cur.second->Destroy();
			delete cur.second;
		}
	}
	m_renderObjList.clear();
	m_instList.clear();
	m_instPerMeshList.clear();
}

uint32_t RenderObjectContainer::GetRenderObjectCount()
{
	return static_cast<uint32_t>(m_renderObjList.size());
}

SimpleRenderObject* RenderObjectContainer::GetRenderObject(uint32_t index)
{
	if (index < m_renderObjList.size())
	{
		auto pos = m_renderObjList.begin();
		std::advance(pos, index);
		return pos->second;
	}
	return nullptr;
}

int RenderObjectContainer::GetRenderObjectBindIndex(SimpleRenderObject* renderObj)
{
	if (renderObj != nullptr)
	{
		auto iterFind = m_instList.find(renderObj->GetUID());
		if (iterFind != m_instList.end())
		{
			return static_cast<int>(std::distance(m_instList.begin(), iterFind));
		}
	}
	return INVALID_INDEX_INT;
}

uint32_t RenderObjectContainer::GetRenderObjectInstanceCount()
{
	return static_cast<uint32_t>(m_instList.size());
}

SampleRenderObjectInstance* RenderObjectContainer::GetRenderObjectInstance(uint32_t index)
{
	if (index < m_instList.size())
	{
		auto pos = m_instList.begin();
		std::advance(pos, index);
		return pos->second;
	}
	return nullptr;
}

int RenderObjectContainer::GetRenderObjectInstanceBindIndex(SampleRenderObjectInstance* inst)
{
	if (inst != nullptr)
	{
		auto iterFind = m_instList.find(inst->GetUID());
		if (iterFind != m_instList.end())
		{
			return static_cast<int>(std::distance(m_instList.begin(), iterFind));
		}
	}
	return INVALID_INDEX_INT;
}

uint32_t RenderObjectContainer::GetRenderObjectInstancePerMeshCount()
{
	return static_cast<uint32_t>(m_instPerMeshList.size());
}

SampleRenderObjectInstancePerMesh* RenderObjectContainer::GetRenderObjectInstancePerMesh(uint32_t index)
{
	if (index < m_instPerMeshList.size())
	{
		auto pos = m_instPerMeshList.begin();
		std::advance(pos, index);
		return pos->second;
	}
	return nullptr;
}

int RenderObjectContainer::GetRenderObjectInstancePerMeshBindIndex(SampleRenderObjectInstancePerMesh* instPerMesh)
{
	if (instPerMesh != nullptr)
	{
		auto iterFind = m_instPerMeshList.find(instPerMesh->GetUID());
		if (iterFind != m_instPerMeshList.end())
		{
			return static_cast<int>(std::distance(m_instPerMeshList.begin(), iterFind));
		}
	}
	return INVALID_INDEX_INT;
}