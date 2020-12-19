#include "GeometryContainer.h"
#include "Utils.h"

SimpleGeometry* GeometryContainer::CreateGeometry(CommandBufferBase* cmdBuffer, std::string& fbxFilePath, bool genMeshlet)
{
	SimpleGeometry* geomData = nullptr;

	auto iterKeyFinded = m_geomKeyTable.find(fbxFilePath);
	if(iterKeyFinded != m_geomKeyTable.end())
	{
		auto iterIdxFinded = m_geomDatas.find(iterKeyFinded->second);
		if (iterIdxFinded != m_geomDatas.end())
		{
			geomData = iterIdxFinded->second;
		}
	}

	if (geomData == nullptr)
	{
		geomData = LoadGeometry(cmdBuffer, fbxFilePath, genMeshlet);
	}

	if (geomData != nullptr)
	{
		geomData->IncRef();
	}
	
	return geomData;
}

int GeometryContainer::GetMeshBindIndex(SimpleMeshData* meshData)
{
	return GetMeshBindIndexFromUID(meshData->GetUID());
}

int GeometryContainer::GetMeshBindIndexFromUID(UID uid)
{
	auto iterFind = m_meshDatas.find(uid);
	if (iterFind != m_meshDatas.end())
	{
		return static_cast<int>(std::distance(m_meshDatas.begin(), iterFind));
	}
	return INVALID_INDEX_INT;
}

void GeometryContainer::RemoveUnusedGeometries()
{
	std::vector<SimpleGeometry*> m_removeList;
	for (auto& cur : m_geomDatas)
	{
		if (cur.second->GetRefCount() == 0)
		{	
			m_removeList.push_back(cur.second);
		}
	}

	for (auto cur : m_removeList)
	{
		UnloadGeometry(cur);
	}
}

void GeometryContainer::Clear()
{
	for (auto& cur : m_geomDatas)
	{
		if (cur.second->GetRefCount() == 0)
		{
			if (cur.second != nullptr)
			{
				cur.second->Unload();
				delete cur.second;
			}
		}
	}
	m_geomDatas.clear();
}

SimpleGeometry* GeometryContainer::GetGeometry(uint32_t index)
{
	if (index < m_geomDatas.size())
	{
		auto pos = m_geomDatas.begin();
		std::advance(pos, index);
		return pos->second;
	}
	return nullptr;
}

SimpleMeshData* GeometryContainer::GetMesh(uint32_t index)
{
	if (index < m_meshDatas.size())
	{
		auto pos = m_meshDatas.begin();
		std::advance(pos, index);
		return pos->second;
	}
	return nullptr;
}

SimpleMeshData* GeometryContainer::GetMeshFromUID(UID uid)
{
	auto iterFind = m_meshDatas.find(uid);
	if (iterFind != m_meshDatas.end())
	{
		return iterFind->second;
	}
	return nullptr;
}

SimpleGeometry* GeometryContainer::LoadGeometry(CommandBufferBase* cmdBuffer, std::string& filePath, bool genMeshlet)
{
	SimpleGeometry* geometry = new SimpleGeometry();
	geometry->Load(cmdBuffer, filePath, genMeshlet);
	UID uid = geometry->GetUID();
	m_geomDatas.insert(std::make_pair(uid, geometry));
	m_geomKeyTable.insert(std::make_pair(filePath, uid));
	return geometry;
}

void GeometryContainer::UnloadGeometry(SimpleGeometry* geomData)
{
	m_geomKeyTable.erase(geomData->GetSrcFilePath());
	auto iterFind = m_geomDatas.find(geomData->GetUID());
	if (iterFind != m_geomDatas.end())
	{
		if (iterFind->second != nullptr)
		{
			iterFind->second->Unload();
			delete iterFind->second;
		}
		m_geomDatas.erase(iterFind);
	}
}

SimpleMeshData* GeometryContainer::LoadMesh(CommandBufferBase* cmdBuffer, FbxGeometryData& fbxGeomData, bool genMeshlet)
{
	SimpleMeshData* meshData = new SimpleMeshData();
	meshData->Load(cmdBuffer, fbxGeomData, genMeshlet);
	m_meshDatas.insert(std::make_pair(meshData->GetUID(), meshData));
	
	OnMeshLoaded.Exec(GetMeshBindIndex(meshData));

	return meshData;
}

void GeometryContainer::UnloadMesh(SimpleMeshData* geomData)
{
	auto iterFind = m_meshDatas.find(geomData->GetUID());
	if (iterFind != m_meshDatas.end())
	{
		uint32_t removeIndex = GetMeshBindIndex(geomData);
		if (iterFind->second != nullptr)
		{
			iterFind->second->Unload();
			delete iterFind->second;
			m_meshDatas.erase(iterFind);
		}
		OnMeshUnloaded.Exec(removeIndex);
	}
}