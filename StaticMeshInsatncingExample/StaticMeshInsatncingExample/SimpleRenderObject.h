#pragma once

#include "GeometryContainer.h"
#include "MaterialContainer.h"
#include "Utils.h"

class SampleRenderObjectInstance;

class SampleRenderObjectInstancePerMesh : public UniqueIdentifier
{
friend class RenderObjectContainer;
public:
	SampleRenderObjectInstancePerMesh() : UniqueIdentifier() {}
public:
	void SetOverrideMaterial(CommandBufferBase* cmdBuffer, ExampleMaterialType type);

	SimpleMeshData* GetMeshData();
	SimpleMaterial* GetMaterial();
	SampleRenderObjectInstance* GetParentInstance() { return m_parentInstance; }

	bool HasOverridMaterial() { return m_overrideMaterial != nullptr; }

protected:
	void Initialize(SampleRenderObjectInstance* parentInstance,  SimpleMeshData* m_meshData, SimpleMaterial* m_material);
	void Destroy();

protected:
	SimpleMeshData* m_meshData = nullptr;
	SimpleMaterial* m_material = nullptr;
	SimpleMaterial* m_overrideMaterial = nullptr;
	SampleRenderObjectInstance* m_parentInstance = nullptr;
};

class SampleRenderObjectInstance : public UniqueIdentifier
{
friend class RenderObjectContainer;
public:
	SampleRenderObjectInstance() : UniqueIdentifier() {}
public:
	void SetOverrideMaterial(CommandBufferBase* cmdBuffer, ExampleMaterialType type);

	uint32_t GetInstancePerMeshCount() { return static_cast<uint32_t>(m_instancePerMesh.size()); }

	SampleRenderObjectInstancePerMesh* GetInstancePerMesh(uint32_t index);
	void CreateInstancePerMesh(SimpleMeshData* meshData, SimpleMaterial* material);

	void SetWorldMatrix(XMMATRIX& matWorld, bool isUpdate);
	XMMATRIX& GetWorldMatrix() { return m_worldMatrix; }

	bool HasOverrideMaterial() { return m_overrideMaterial != nullptr; }
	SimpleMaterial* GetOverrideMaterial() { return m_overrideMaterial; }

	LambdaCommandListWithOneParam<std::function<void(uint32_t)>, uint32_t> OnInstanceUpdated;

protected:
	void Initialzie(XMMATRIX& worldMat);
	void Destroy();

private:
	XMMATRIX m_worldMatrix = {};
	std::vector<SampleRenderObjectInstancePerMesh*> m_instancePerMesh = {};
	SimpleMaterial* m_overrideMaterial = nullptr;
};

class SimpleRenderObject : public UniqueIdentifier
{
friend class RenderObjectContainer;
public:
	SimpleRenderObject() : UniqueIdentifier() {}
public:
	SampleRenderObjectInstance* CreateInstance(XMMATRIX& matWorld);
	void RemoveInstance(SampleRenderObjectInstance* subMeshInstance);

	uint32_t GetInstanceCount() { return static_cast<uint32_t>(m_instances.size()); } 
	SampleRenderObjectInstance* GetInstance(uint32_t index);

	SimpleGeometry* GetGeometry() { return m_geometry; }
	SimpleMaterial* GetMaterial() { return m_material; }
	
	void CollectMaterials(std::vector<SimpleMaterial*>& buffer);

protected:
	void Initialize(CommandBufferBase* cmdBuffer, std::string fbxFilePath, ExampleMaterialType exampleMaterialType, bool useMeshShader = false);
	void Destroy();

private:

	SimpleGeometry* m_geometry = nullptr;
	SimpleMaterial* m_material = nullptr;
	uint32_t m_materialIndex = 0;

	std::map<UID, uint32_t> m_indexTable = {};
	std::vector<SampleRenderObjectInstance*> m_instances = {};
};