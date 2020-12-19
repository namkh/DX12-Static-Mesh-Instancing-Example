#pragma once

#include "SimpleRenderObject.h"
#include "Singleton.h"
#include <unordered_map>

class RenderObjectContainer : public TSingleton<RenderObjectContainer>
{
public:
	RenderObjectContainer(token) 
	{
	};

public:
	SimpleRenderObject* CreateRenderObject(CommandBufferBase* cmdBuffer, std::string fbxFilePath, ExampleMaterialType exampleMaterialType, bool useMeshShader = false);
	void RemoveRenderObject(SimpleRenderObject* obj);

	SampleRenderObjectInstance* CreateRenderObjectInstance(XMMATRIX& matWorld);
	void RemoveRenderObjectInstance(SampleRenderObjectInstance* instance);

	SampleRenderObjectInstancePerMesh* CreateRenderObjectInstancePerMesh(SampleRenderObjectInstance* parentInst, SimpleMeshData* meshData, SimpleMaterial* material);
	void RemoveRenderObjectInstancePerMesh(SampleRenderObjectInstancePerMesh* instancePreMesh);

	void Clear();

public:

	uint32_t GetRenderObjectCount();
	SimpleRenderObject* GetRenderObject(uint32_t index);
	int GetRenderObjectBindIndex(SimpleRenderObject* renderObj);

	uint32_t GetRenderObjectInstanceCount();
	SampleRenderObjectInstance* GetRenderObjectInstance(uint32_t index);
	int GetRenderObjectInstanceBindIndex(SampleRenderObjectInstance* inst);

	uint32_t GetRenderObjectInstancePerMeshCount();
	SampleRenderObjectInstancePerMesh* GetRenderObjectInstancePerMesh(uint32_t index);
	int GetRenderObjectInstancePerMeshBindIndex(SampleRenderObjectInstancePerMesh* instPerMesh);

	bool IsDirty() { return m_isDirty; }
	void SetDirty(bool isDirty) { m_isDirty = true; }

	LambdaCommandListWithOneParam<std::function<void(uint32_t)>, uint32_t> OnInstPerMeshAdded;
	LambdaCommandListWithOneParam<std::function<void(uint32_t)>, uint32_t> OnInstPerMeshRemoved;

protected:

	template <typename CreateItemType, typename ListType>
	CreateItemType* Create(ListType* idxList);

	template <typename RemoveItemType, typename ListType>
	void Remove(RemoveItemType* item, ListType* idxList);

private:

	std::unordered_map<UID, SimpleRenderObject*> m_renderObjList;
	std::unordered_map<UID, SampleRenderObjectInstance*> m_instList;
	std::unordered_map<UID, SampleRenderObjectInstancePerMesh*> m_instPerMeshList;

	bool m_isDirty = false;
};

template <typename CreateItemType, typename ListType>
CreateItemType* RenderObjectContainer::Create(ListType* itemList)
{
	CreateItemType* item = new CreateItemType();
	itemList->insert(std::make_pair(item->GetUID(), item));
	m_isDirty = true;
	return item;
}

template <typename RemoveItemType, typename ListType>
void RenderObjectContainer::Remove(RemoveItemType* item, ListType* itemList)
{
	auto iterIdxFind = itemList->find(item->GetUID());
	if (iterIdxFind != itemList->end())
	{
		if (iterIdxFind->second != nullptr)
		{
			iterIdxFind->second->Destroy();
			delete iterIdxFind->second;
			itemList->erase(iterIdxFind);
			m_isDirty = true;
		}
	}
}
#define gRenderObjContainer RenderObjectContainer::Instance()