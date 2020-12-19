#pragma once

#include <fbxsdk.h>
#include<vector>

#include "Defaults.h"
#include "Singleton.h"

struct FbxGeometryData
{
	std::vector<XMFLOAT3> m_positions;
	std::vector<uint32_t> m_indices;
	std::vector<XMFLOAT3> m_normals;
	std::vector<XMFLOAT3> m_tangents;
	std::vector<XMFLOAT3> m_bitangents;
	std::vector<XMFLOAT4> m_color;
	std::vector<XMFLOAT2> m_uv;
};


class SimpleFbxGeometiesLoader : public TSingleton<SimpleFbxGeometiesLoader>
{
public:
	SimpleFbxGeometiesLoader(token) {};

public:
	bool Initialize();
	void Destory();

protected:
	void GetGeometries(fbxsdk::FbxNode* curNode, std::vector<fbxsdk::FbxMesh*>& fbxMeshes);

	void ReadNormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outNormal);
	void ReadTangent(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outTangent);
	void ReadUV(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT2& outUV);
	void ReadColor(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT4& outColor);

public:
	void Load(std::string filePath, std::vector<FbxGeometryData>& fbxGeometryDatas, bool regenNormalAndTangent = false);

private:

	fbxsdk::FbxManager* m_fbxManager = nullptr;
	fbxsdk::FbxIOSettings* m_fbxIoSettings = nullptr;
};

#define gFbxGeomLoader SimpleFbxGeometiesLoader::Instance()