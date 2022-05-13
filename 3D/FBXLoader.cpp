#include "FBXLoader.h"
#include <cassert>

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
}
