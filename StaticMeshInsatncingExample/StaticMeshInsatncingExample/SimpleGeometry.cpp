#include "SimpleGeometry.h"
#include "GeometryContainer.h"

#include <map>

bool Meshlet::LimitCheck(uint32_t* tris)
{
	if (GetPrimCount() + 3 > MESHLET_MAX_PRIMS)
	{
		return false;
	}

	uint32_t numOfAddVerts = 0;
	for (uint8_t i = 0; i < 3; i++)
	{
		uint32_t localIndex = tris[i] - m_vertIdxOffset;
		auto iterFind = m_meshletIndicesMatchTable.find(localIndex);
		if (iterFind == m_meshletIndicesMatchTable.end())
		{
			numOfAddVerts++;
		}
	}
	if (static_cast<uint32_t>(m_meshletLocalVertexIndices.size()) + numOfAddVerts > MESHLET_MAX_VERTS)
	{
		return false;
	}
	return true;
}

void Meshlet::AddTriangle(uint32_t* tris)
{
	uint32_t primIdx[3] = { 0, 0, 0 };
	uint32_t locPrimIdx[3] = { 0, 0, 0 };
	for (uint8_t i = 0; i < 3; i++)
	{
		uint32_t vertIndex = tris[i];
		auto iterFind = m_meshletIndicesMatchTable.find(vertIndex);
		if (iterFind == m_meshletIndicesMatchTable.end())
		{
			m_meshletLocalVertexIndices.push_back(vertIndex);
			uint32_t addedIndex = static_cast<uint32_t>(m_meshletLocalVertexIndices.size() - 1);
			m_meshletIndicesMatchTable.insert(std::make_pair(vertIndex, addedIndex));
			locPrimIdx[i] = static_cast<uint32_t>(addedIndex);
		}
		else
		{
			locPrimIdx[i] = iterFind->second;
		}
	}
	LocPrimIndex data = { locPrimIdx[0], locPrimIdx[1], locPrimIdx[2] };
	m_meshletLocalPrimitiveIndices.push_back(data);
}

void Meshlet::BuildCullData(std::vector<DefaultVertex>& verts)
{
	XMVECTOR normalAvg = {};
	XMVECTOR center = {};
	for (int i = 0; i < m_meshletLocalVertexIndices.size(); i++)
	{
		center = XMVectorAdd(center, XMLoadFloat3(&verts[m_meshletLocalVertexIndices[i]].m_position));
		normalAvg = XMVectorAdd(normalAvg, XMLoadFloat3(&verts[m_meshletLocalVertexIndices[i]].m_normal));
	}

	normalAvg = XMVector3Normalize(normalAvg);
	XMStoreFloat3(&m_cullData.m_direction, normalAvg);

	float numVerts = static_cast<float>(verts.size());
	XMVECTOR vecNumVerts = XMLoadFloat(&numVerts);
	center = XMVectorDivide(center, vecNumVerts);
	XMStoreFloat3(&m_cullData.m_centerPosition, center);

	XMFLOAT3 minVec = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
	XMVECTOR maxAngle = XMLoadFloat3(&minVec);

	for (int i = 0; i < m_meshletLocalVertexIndices.size(); i++)
	{
		XMVECTOR dot = XMVector3Dot(normalAvg, XMLoadFloat3(&verts[m_meshletLocalVertexIndices[i]].m_normal));
		maxAngle = XMVectorMax(dot, maxAngle);
	}

	m_cullData.m_seta = XMVectorGetX(maxAngle);
}

void Meshlet::GetMeshletData(MeshletData& meshletData)
{
	meshletData.VertexIndexCount = static_cast<uint32_t>(m_meshletLocalVertexIndices.size());
	meshletData.VertexIndexOffset = m_vertIdxOffset;
	meshletData.PrimitiveCount = static_cast<uint32_t>(m_meshletLocalPrimitiveIndices.size());
	meshletData.PrimitiveOffset = m_primIdxOffset;
}

bool SimpleMeshData::Load(CommandBufferBase* cmdBuffer, FbxGeometryData& geometryData, bool genMeshlet)
{
	std::vector<DefaultVertex> verts(geometryData.m_positions.size());
	std::vector<uint32_t> indices(geometryData.m_indices);

	for (int i = 0; i < geometryData.m_positions.size(); i++)
	{
		verts[i].m_position = geometryData.m_positions[i];
		if (geometryData.m_normals.size() != 0)
		{
			verts[i].m_normal = geometryData.m_normals[i];
		}
		if (geometryData.m_tangents.size() != 0)
		{
			verts[i].m_tangent = geometryData.m_tangents[i];
		}
		if (geometryData.m_color.size() != 0)
		{
			verts[i].m_color = geometryData.m_color[i];
		}
		if (geometryData.m_uv.size() != 0)
		{
			verts[i].m_texcoord = geometryData.m_uv[i];
		}
	}

	if (genMeshlet)
	{
		uint32_t vertLocIdxOffset = 0;
		uint32_t primOffset = 0;

		std::vector<Meshlet*> meshlets;
		Meshlet* curMeshlet = new Meshlet(vertLocIdxOffset, primOffset);
		meshlets.push_back(curMeshlet);

		uint32_t tris[3] = { 0, 0, 0 };
		for (uint32_t i = 0; i < indices.size(); i += 3)
		{
			tris[0] = indices[i];
			tris[1] = indices[i + 1];
			tris[2] = indices[i + 2];
			if (!curMeshlet->LimitCheck(tris))
			{
				vertLocIdxOffset += curMeshlet->GetVertIdxCount();
				primOffset += curMeshlet->GetPrimCount();
				curMeshlet = new Meshlet(vertLocIdxOffset, primOffset);
				meshlets.push_back(curMeshlet);
			}
			curMeshlet->AddTriangle(tris);
		}

		Meshlet* lastMeshlet = meshlets[meshlets.size() - 1];
		m_meshletInfo.MeshletCount = static_cast<uint32_t>(meshlets.size());
		m_meshletInfo.LastMeshletVertIdxCount = lastMeshlet->GetVertIdxCount();
		m_meshletInfo.LastMeshletPrimCount = lastMeshlet->GetPrimCount();

		std::vector<uint32_t> vertIdxList;
		std::vector<LocPrimIndex> primIdxList;
		std::vector<MeshletData> meshletDatas(meshlets.size());
		for (uint32_t i = 0; i < meshletDatas.size(); i++)
		{
			meshlets[i]->BuildCullData(verts);
			meshlets[i]->GetMeshletData(meshletDatas[i]);

			std::vector<uint32_t>& curMeshletVertIdxs = meshlets[i]->GetLocalVertexIndices();
			std::vector<LocPrimIndex>& curMeshletPrimIdxs = meshlets[i]->GetLocalPrimitiveIndices();

			vertIdxList.insert(vertIdxList.end(), curMeshletVertIdxs.begin(), curMeshletVertIdxs.end());
			primIdxList.insert(primIdxList.end(), curMeshletPrimIdxs.begin(), curMeshletPrimIdxs.end());
		}

		if (!m_locVertIndexBuffer.Initialize(cmdBuffer, vertIdxList))
		{
			return false;
		}

		if (!m_locPrimIndexBuffer.Initialize(static_cast<uint32_t>(primIdxList.size()), false))
		{
			return false;
		}
		m_locPrimIndexBuffer.UploadResource(cmdBuffer, static_cast<void*>(primIdxList.data()));

		if (!m_meshletBuffer.Initialize(static_cast<uint32_t>(meshletDatas.size()), false))
		{
			return false;
		}
		m_meshletBuffer.UploadResource(cmdBuffer, static_cast<void*>(meshletDatas.data()));

		m_hasMeshlet = true;

		for (auto& cur : meshlets)
		{
			if (cur != nullptr)
			{
				delete cur;
			}
		}
		meshlets.clear();
	}

	if (!m_vertexBuffer.Initialize(cmdBuffer, verts))
	{
		return false;
	}
	if (!m_indexBuffer.Initialize(cmdBuffer, indices))
	{
		return false;
	}

	return true;
}

void SimpleMeshData::Unload()
{
	gDX12DeviceRes->WaitForCommandQueue();

	m_vertexBuffer.Destroy();
	m_indexBuffer.Destroy();
	if (m_hasMeshlet)
	{
		m_locVertIndexBuffer.Destroy();
		m_locPrimIndexBuffer.Destroy();
		m_meshletBuffer.Destroy();
	}
}

void SimpleMeshData::OnUpdated()
{
	if (OnMeshUpdated.IsBinded())
	{
		int index = gGeomContainer.GetMeshBindIndex(this);
		if (index != -1)
		{
			OnMeshUpdated.Exec(static_cast<uint32_t>(index));
		}
	}
}

void SimpleGeometry::Destroy()
{
	DecRef();
}

SimpleMeshData* SimpleGeometry::GetMesh(uint32_t index)
{
	if (index < m_meshList.size())
	{
		return m_meshList[index];
	}
	return nullptr;
}

std::vector<D3D12_INPUT_ELEMENT_DESC>* SimpleGeometry::GetDefaultInputLayout()
{
	SimpleMeshData* firstMeshData = GetMesh(0);
	if (firstMeshData != nullptr)
	{
		return firstMeshData->GetInputLayout();
	}
	return nullptr;
}

bool SimpleGeometry::Load(CommandBufferBase* cmdBuffer, std::string& fbxFilePath, bool genMeshlet)
{
	std::vector<FbxGeometryData> geomDatas;
	gFbxGeomLoader.Load(fbxFilePath, geomDatas);

	for (auto& cur : geomDatas)
	{
		SimpleMeshData* meshData = gGeomContainer.LoadMesh(cmdBuffer, cur, genMeshlet);
		m_meshList.push_back(meshData);
	}
	m_srcFilePath = fbxFilePath;
	SetSrcFilePath(fbxFilePath);

	return true;
}

void SimpleGeometry::Unload()
{
	for (auto& cur : m_meshList)
	{
		gGeomContainer.UnloadMesh(cur);
	}
	m_meshList.clear();
}