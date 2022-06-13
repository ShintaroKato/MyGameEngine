#include "FBXLoader.h"
#include <cassert>

using namespace DirectX;

const std::string FBXLoader::baseDirectory = "Resources/";
const std::string FBXLoader::defaultTextureFileName = "white1x1.png";

FBXLoader* FBXLoader::GetInstance()
{
	static FBXLoader instance;
	return &instance;
}

void FBXLoader::Initialize(ID3D12Device* device)
{
	assert(fbxManager == nullptr);

	this->device = device;

	fbxManager = FbxManager::Create();

	FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(ios);

	fbxImporter = FbxImporter::Create(fbxManager, "");
}

void FBXLoader::Finalize()
{
	fbxImporter->Destroy();
	fbxManager->Destroy();
}

FBXModel* FBXLoader::LoadModelFromFile(const string& modelName)
{
	const string directoryPath = baseDirectory + modelName + "/";

	const string fileName = modelName + ".fbx";

	const string fullPath = directoryPath + fileName;

	if (!fbxImporter->Initialize(fullPath.c_str(),
		-1, fbxManager->GetIOSettings()))
	{
		assert(0);
	}

	FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxScene");

	fbxImporter->Import(fbxScene);
	fbxImporter->Import(fbxScene);

	FBXModel* fbxModel = new FBXModel();
	fbxModel->name = modelName;

	int nodeCount = fbxScene->GetNodeCount();
	fbxModel->node.reserve(nodeCount);

	ParseNode(fbxModel, fbxScene->GetRootNode());

	fbxScene->Destroy();

	fbxModel->CreateBuffers(device);

	return fbxModel;
}

void FBXLoader::ParseNode(FBXModel* fbxModel, FbxNode* fbxNode, Node* parent)
{
	string name = fbxNode->GetName();

	fbxModel->node.emplace_back();
	Node& node_ = fbxModel->node.back();

	node_.name = fbxNode->GetName();

	FbxDouble3 rotation = fbxNode->LclRotation.Get();
	FbxDouble3 scaling = fbxNode->LclScaling.Get();
	FbxDouble3 translation = fbxNode->LclTranslation.Get();

	node_.rotation = { (float)rotation[0],(float)rotation[1], (float)rotation[2],0.0f };
	node_.scaling = { (float)scaling[0],(float)scaling[1], (float)scaling[2], 0.0f };
	node_.translation = { (float)translation[0],(float)translation[1], (float)translation[2], 1.0f };

	node_.rotation.m128_f32[0] = XMConvertToRadians(node_.rotation.m128_f32[0]);
	node_.rotation.m128_f32[1] = XMConvertToRadians(node_.rotation.m128_f32[1]);
	node_.rotation.m128_f32[2] = XMConvertToRadians(node_.rotation.m128_f32[2]);

	XMMATRIX matScaling, matRotation, matTranslation;
	matScaling = XMMatrixScalingFromVector(node_.scaling);
	matRotation = XMMatrixRotationRollPitchYawFromVector(node_.rotation);
	matTranslation = XMMatrixTranslationFromVector(node_.translation);

	node_.transform = XMMatrixIdentity();
	node_.transform *= matScaling;
	node_.transform *= matRotation;
	node_.transform *= matTranslation;

	node_.globalTransform = node_.transform;

	if (parent)
	{
		node_.parent = parent;
		node_.globalTransform *= parent->globalTransform;
	}

	FbxNodeAttribute* fbxNodeAttr = fbxNode->GetNodeAttribute();

	if (fbxNodeAttr)
	{
		if (fbxNodeAttr->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			fbxModel->meshNode = &node_;
			ParseMesh(fbxModel, fbxNode);
		}
	}

	for (int i = 0; i < fbxNode->GetChildCount(); i++)
	{
		ParseNode(fbxModel, fbxNode->GetChild(i), &node_);
	}
}

void FBXLoader::ParseMesh(FBXModel* fbxModel, FbxNode* fbxNode)
{
	FbxMesh* fbxMesh = fbxNode->GetMesh();

	ParseVertex(fbxModel, fbxMesh);
	ParseFace(fbxModel, fbxMesh);
	ParseMaterial(fbxModel, fbxNode);

}

void FBXLoader::ParseVertex(FBXModel* fbxModel, FbxMesh* fbxMesh)
{
	auto& vertices = fbxModel->vertices;

	const int controlPointCount =
		fbxMesh->GetControlPointsCount();

	FBXModel::VertexPosNormalUv vert{};
	fbxModel->vertices.resize(controlPointCount, vert);

	FbxVector4* pCoord = fbxMesh->GetControlPoints();

	for (int i = 0; i < controlPointCount; i++)
	{
		FBXModel::VertexPosNormalUv& vertex = vertices[i];

		vertex.pos.x = (float)pCoord[i][0];
		vertex.pos.y = (float)pCoord[i][1];
		vertex.pos.z = (float)pCoord[i][2];
	}
}

void FBXLoader::ParseFace(FBXModel* fbxModel, FbxMesh* fbxMesh)
{
	auto& vertices = fbxModel->vertices;
	auto& indices = fbxModel->indices;

	assert(indices.size() == 0);

	const int polygonCount = fbxMesh->GetPolygonCount();
	const int texUVCount = fbxMesh->GetTextureUVCount();

	FbxStringList uvNames;
	fbxMesh->GetUVSetNames(uvNames);

	for (int i = 0; i < polygonCount; i++)
	{
		const int polygonSize = fbxMesh->GetPolygonSize(i);
		assert(polygonSize <= 4);

		for (int j = 0; j < polygonSize; j++)
		{
			int index = fbxMesh->GetPolygonVertex(i, j);
			assert(index >= 0);

			FBXModel::VertexPosNormalUv& vertex = vertices[index];
			FbxVector4 normal;

			if (fbxMesh->GetPolygonVertexNormal(i, j, normal))
			{
				vertex.normal.x = (float)normal[0];
				vertex.normal.y = (float)normal[1];
				vertex.normal.z = (float)normal[2];
			}

			if (texUVCount > 0)
			{
				FbxVector2 uvs;
				bool lUnmappedUV;

				if (fbxMesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmappedUV))
				{
					vertex.uv.x = (float)uvs[0];
					vertex.uv.y = (float)uvs[1];
				}
			}

			if (j < 3)
			{
				indices.push_back(index);
			}
			else
			{
				int index2 = indices[indices.size() - 1];
				int index3 = index;
				int index0 = indices[indices.size() - 3];

				indices.push_back(index2);
				indices.push_back(index3);
				indices.push_back(index0);
			}
		}
	}
}

void FBXLoader::ParseMaterial(FBXModel* fbxModel, FbxNode* fbxNode)
{
	const int materialCount = fbxNode->GetMaterialCount();
	if (materialCount > 0)
	{
		FbxSurfaceMaterial* material = fbxNode->GetMaterial(0);

		bool textureLoaded = false;

		if (material)
		{
			FbxSurfaceLambert* lambert =
				static_cast<FbxSurfaceLambert*>(material);

			FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
			fbxModel->ambient.x = (float)ambient.Get()[0];
			fbxModel->ambient.y = (float)ambient.Get()[1];
			fbxModel->ambient.z = (float)ambient.Get()[2];

			FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
			fbxModel->diffuse.x = (float)diffuse.Get()[0];
			fbxModel->diffuse.y = (float)diffuse.Get()[1];
			fbxModel->diffuse.z = (float)diffuse.Get()[2];

			const FbxProperty diffuseProperty =
				material->FindProperty(FbxSurfaceMaterial::sDiffuse);

			if (diffuseProperty.IsValid())
			{
				const FbxFileTexture* texture = diffuseProperty.GetSrcObject<FbxFileTexture>();

				if (texture)
				{
					const char* filepath = texture->GetFileName();

					string path_str(filepath);
					string name = ExtractFileName(path_str);

					LoadTexture(fbxModel, baseDirectory + fbxModel->name + "/" + name);
					textureLoaded = true;
				}
			}
		}

		if (!textureLoaded)
		{
			LoadTexture(fbxModel, baseDirectory + defaultTextureFileName);
		}
	}
}

void FBXLoader::LoadTexture(FBXModel* fbxModel, const std::string& fullpath)
{
	HRESULT result = S_FALSE;

	TexMetadata& metadata = fbxModel->metadata;
	ScratchImage& scratchImg = fbxModel->scratchImg;

	wchar_t wfilepath[128];
	MultiByteToWideChar(CP_ACP, 0, fullpath.c_str(), -1, wfilepath, _countof(wfilepath));

	result = LoadFromWICFile(
		wfilepath, WIC_FLAGS_NONE,
		&metadata, scratchImg);
	if (FAILED(result))
	{
		assert(0);
	}
}

std::string FBXLoader::ExtractFileName(const std::string& path)
{
	size_t pos1;

	pos1 = path.rfind('\\');
	if (pos1 != string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}

	pos1 = path.rfind('/');
	if (pos1 != string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}

	return path;
}