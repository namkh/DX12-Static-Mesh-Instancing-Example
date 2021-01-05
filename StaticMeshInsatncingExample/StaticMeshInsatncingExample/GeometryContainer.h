#pragma once

#include <unordered_map>
#include <functional>

#include "DeviceBuffers.h"
#include "Singleton.h"
#include "SimpleGeometry.h"
#include "Commands.h"

class GeometryContainer : public TSingleton<GeometryContainer>
{
friend class SimpleGeometry;
public:
	GeometryContainer(token) 
	{
	};
	
public:
	SimpleGeometry* CreateGeometry(CommandBufferBase* cmdBuffer, std::string& filePath, bool genMeshlet = false);
	int GetMeshBindIndex(SimpleMeshData* meshData);
	int GetMeshBindIndexFromUID(UID uid);

	void RemoveUnusedGeometries();
	void Clear();

public:
	uint32_t GetGeometryCount() { return static_cast<uint32_t>(m_geomDatas.size()); }
	SimpleGeometry* GetGeometry(uint32_t index);

	uint32_t GetMeshCount() { return static_cast<uint32_t>(m_meshDatas.size()); }
	SimpleMeshData* GetMesh(uint32_t index);
	SimpleMeshData* GetMeshFromUID(UID uid);


protected:

	SimpleGeometry* LoadGeometry(CommandBufferBase* cmdBuffer, std::string& filePath, bool genMeshlet = false);
	void UnloadGeometry(SimpleGeometry* geomData);

	SimpleMeshData* LoadMesh(CommandBufferBase* cmdBuffer, FbxGeometryData& fbxGeomData, bool genMeshlet = false);
	void UnloadMesh(SimpleMeshData* geomData);

public:

	LambdaCommandListWithOneParam<std::function<void(uint32_t)>, uint32_t> OnMeshLoaded;
	LambdaCommandListWithOneParam<std::function<void(uint32_t)>, uint32_t> OnMeshUnloaded;
	
private:
	std::map<std::string, UID> m_geomKeyTable;
	std::unordered_map<UID, SimpleGeometry*> m_geomDatas;
	std::unordered_map<UID, SimpleMeshData*> m_meshDatas;
};

#define gGeomContainer GeometryContainer::Instance()

