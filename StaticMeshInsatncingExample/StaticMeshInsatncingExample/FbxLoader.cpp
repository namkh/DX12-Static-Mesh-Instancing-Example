#include "FbxLoader.h"

bool SimpleFbxGeometiesLoader::Initialize()
{
	m_fbxManager = FbxManager::Create();
	m_fbxIoSettings = FbxIOSettings::Create(m_fbxManager, IOSROOT);
	m_fbxManager->SetIOSettings(m_fbxIoSettings);

	return true;
}

void SimpleFbxGeometiesLoader::Destory()
{
	m_fbxIoSettings->Destroy();
	m_fbxManager->Destroy();
}

void SimpleFbxGeometiesLoader::GetGeometries(fbxsdk::FbxNode* curNode, std::vector<fbxsdk::FbxMesh*>& fbxMeshes)
{
	if (curNode != nullptr)
	{
		fbxsdk::FbxMesh* mesh = curNode->GetMesh();
		if (mesh != nullptr)
		{
			fbxMeshes.push_back(mesh);
		}
		for (int i = 0; i < curNode->GetChildCount(); i++)
		{
			fbxsdk::FbxNode* childNode = curNode->GetChild(i);
			GetGeometries(childNode, fbxMeshes);
		}
	}
}

void SimpleFbxGeometiesLoader::ReadNormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outNormal)
{
	fbxsdk::FbxGeometryElementNormal* vertexNormal = inMesh->GetElementNormal(0);

	switch (vertexNormal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[2]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(inCtrlPointIndex);
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
		}
		break;
		default:
			throw std::exception("Invalid Reference");
		}
		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[2]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[1]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(inVertexCounter);
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
		}
		break;
		default: throw std::exception("Invalid Reference");
		}
		break;
	}
}

void SimpleFbxGeometiesLoader::ReadTangent(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outTangent)
{
	fbxsdk::FbxGeometryElementTangent* vertexTangent = inMesh->GetElementTangent(0);

	switch (vertexTangent->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outTangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
			outTangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inCtrlPointIndex).mData[2]);
			outTangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexTangent->GetIndexArray().GetAt(inCtrlPointIndex);
			outTangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
			outTangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
			outTangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
		}
		break;
		}
		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outTangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inVertexCounter).mData[0]);
			outTangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inVertexCounter).mData[2]);
			outTangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(inVertexCounter).mData[1]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexTangent->GetIndexArray().GetAt(inVertexCounter);
			outTangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
			outTangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
			outTangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
		}
		}
		break;
	}
}

void SimpleFbxGeometiesLoader::ReadUV(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT2& outUV)
{
	fbxsdk::FbxGeometryElementUV* vertexUV = inMesh->GetElementUV(0);

	switch (vertexUV->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
			outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexUV->GetIndexArray().GetAt(inCtrlPointIndex);
			outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
			outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
		}
		break;
		}
		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(inVertexCounter).mData[0]);
			outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(inVertexCounter).mData[1]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexUV->GetIndexArray().GetAt(inVertexCounter);
			outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
			outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
		}
		}
		break;
	}
}

void SimpleFbxGeometiesLoader::ReadColor(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT4& outColor)
{
	if (inMesh->GetElementVertexColorCount() == 0)
	{
		return;
	}

	fbxsdk::FbxGeometryElementVertexColor* vertexColor = inMesh->GetElementVertexColor(0);
	switch (vertexColor->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexColor->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outColor.x = static_cast<float>(vertexColor->GetDirectArray().GetAt(inCtrlPointIndex).mRed);
			outColor.y = static_cast<float>(vertexColor->GetDirectArray().GetAt(inCtrlPointIndex).mGreen);
			outColor.z = static_cast<float>(vertexColor->GetDirectArray().GetAt(inCtrlPointIndex).mBlue);
			outColor.w = static_cast<float>(vertexColor->GetDirectArray().GetAt(inCtrlPointIndex).mAlpha);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexColor->GetIndexArray().GetAt(inCtrlPointIndex);
			outColor.x = static_cast<float>(vertexColor->GetDirectArray().GetAt(index).mRed);
			outColor.y = static_cast<float>(vertexColor->GetDirectArray().GetAt(index).mGreen);
			outColor.z = static_cast<float>(vertexColor->GetDirectArray().GetAt(index).mBlue);
			outColor.w = static_cast<float>(vertexColor->GetDirectArray().GetAt(index).mAlpha);
		}
		break;
		}
		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexColor->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outColor.x = static_cast<float>(vertexColor->GetDirectArray().GetAt(inVertexCounter).mRed);
			outColor.y = static_cast<float>(vertexColor->GetDirectArray().GetAt(inVertexCounter).mGreen);
			outColor.z = static_cast<float>(vertexColor->GetDirectArray().GetAt(inVertexCounter).mBlue);
			outColor.w = static_cast<float>(vertexColor->GetDirectArray().GetAt(inVertexCounter).mAlpha);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexColor->GetIndexArray().GetAt(inVertexCounter);
			outColor.x = static_cast<float>(vertexColor->GetDirectArray().GetAt(index).mRed);
			outColor.y = static_cast<float>(vertexColor->GetDirectArray().GetAt(index).mGreen);
			outColor.z = static_cast<float>(vertexColor->GetDirectArray().GetAt(index).mBlue);
			outColor.w = static_cast<float>(vertexColor->GetDirectArray().GetAt(index).mAlpha);
		}
		break;
		}
		break;
	}
}

void SimpleFbxGeometiesLoader::Load(std::string filePath, std::vector<FbxGeometryData>& fbxGeometryDatas, bool regenNormalAndTangent)
{
	fbxsdk::FbxScene* fbxScene = FbxScene::Create(m_fbxManager, "LoadScene");

	FbxImporter* fbxImpoter = FbxImporter::Create(m_fbxManager, "LoadImporter");
	fbxImpoter->Initialize(filePath.c_str());
	fbxImpoter->Import(fbxScene);

	fbxsdk::FbxNode* rootNode = fbxScene->GetRootNode();
	if (rootNode != nullptr)
	{
		std::vector<fbxsdk::FbxMesh*> fbxMeshList;
		GetGeometries(rootNode, fbxMeshList);
		if (fbxMeshList.size() > 0)
		{
			fbxGeometryDatas.resize(fbxMeshList.size());
			for (int i = 0; i < fbxMeshList.size(); i++)
			{
				uint32_t controlpointCount = fbxMeshList[i]->GetControlPointsCount();
				int numPolygons = fbxMeshList[i]->GetPolygonCount();

				std::vector<bool> writeTable;
				writeTable.resize(controlpointCount, false);

				fbxGeometryDatas[i].m_positions.resize(controlpointCount);
				fbxGeometryDatas[i].m_normals.resize(controlpointCount);
				fbxGeometryDatas[i].m_tangents.resize(controlpointCount);
				fbxGeometryDatas[i].m_bitangents.resize(controlpointCount);
				fbxGeometryDatas[i].m_color.resize(controlpointCount);
				fbxGeometryDatas[i].m_uv.resize(controlpointCount);

				if (fbxMeshList[i]->GetElementNormalCount() == 0 || regenNormalAndTangent)
				{
					fbxMeshList[i]->GenerateNormals(true);
				}

				if (fbxMeshList[i]->GetElementTangentCount() == 0 || regenNormalAndTangent)
				{
					fbxMeshList[i]->GenerateTangentsDataForAllUVSets(true);
				}

				int vertexCounter = 0;
				//index
				for (int j = 0; j < numPolygons; j++)
				{
					for (int k = 0; k < 3; k++)
					{
						int ctrlPointIdx = fbxMeshList[i]->GetPolygonVertex(j, k);
						fbxGeometryDatas[i].m_indices.push_back(ctrlPointIdx);
						if (!writeTable[ctrlPointIdx])
						{
							fbxGeometryDatas[i].m_positions[ctrlPointIdx] = XMFLOAT3(static_cast<float>(fbxMeshList[i]->GetControlPointAt(ctrlPointIdx).mData[0]),
																					 static_cast<float>(fbxMeshList[i]->GetControlPointAt(ctrlPointIdx).mData[2]),
																					 static_cast<float>(fbxMeshList[i]->GetControlPointAt(ctrlPointIdx).mData[1]));

							ReadNormal(fbxMeshList[i], ctrlPointIdx, vertexCounter, fbxGeometryDatas[i].m_normals[ctrlPointIdx]);
							ReadTangent(fbxMeshList[i], ctrlPointIdx, vertexCounter, fbxGeometryDatas[i].m_tangents[ctrlPointIdx]);
							ReadColor(fbxMeshList[i], ctrlPointIdx, vertexCounter, fbxGeometryDatas[i].m_color[ctrlPointIdx]);
							ReadUV(fbxMeshList[i], ctrlPointIdx, vertexCounter, fbxGeometryDatas[i].m_uv[ctrlPointIdx]);
							writeTable[ctrlPointIdx] = true;
						}
						++vertexCounter;
					}
				}
			}
		}
	}
	else
	{
		//비어있다는 로깅
	}

	fbxImpoter->Destroy();
	fbxScene->Destroy();
}
