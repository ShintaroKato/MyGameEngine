#include "FBXLoader.h"
#include <cassert>

using namespace DirectX;

const std::string FBXLoader::baseDirectory = "Resources/";

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

void FBXLoader::LoadModelFromFile(const string& modelName)
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

	for (int i = 0; i < fbxNode->GetChildCount(); i++)
	{
		ParseNode(fbxModel, fbxNode->GetChild(i), &node_);
	}
}

void FBXLoader::ParseMesh(FBXModel* fbxModel, FbxNode* fbxNode)
{
}

void FBXLoader::ParseVertex(FBXModel* fbxModel, FbxMesh* fbxMesh)
{
}

void FBXLoader::ParseMeshFace(FBXModel* fbxModel, FbxMesh* fbxMesh)
{
}

void FBXLoader::ParseMaterial(FBXModel* fbxModel, FbxMesh* fbxMesh)
{
}

void FBXLoader::LoadTexture(FBXModel* fbxModel, const std::string& fullpath)
{
}
