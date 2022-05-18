#pragma once

#include "fbxsdk.h"

#include <d3d12.h>
#include <d3dx12.h>

#include <string>
#include "FBXModel.h"

class FBXLoader
{
private:// �G�C���A�X
	using string = std::string;

public:
	// ���f���i�[���[�g�p�X
	static const string baseDirectory;

public:
	/// <summary>
	/// �V���O���g���C���X�^���X�̎擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static FBXLoader* GetInstance();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	void Initialize(ID3D12Device* device);

	/// <summary>
	/// ��n��
	/// </summary>
	void Finalize();

	/// <summary>
	/// �t�@�C������FBX���f���ǂݍ���
	/// </summary>
	/// <param name="modelName">���f����</param>
	void LoadModelFromFile(const string& modelName);

	/// <summary>
	/// �ċA�I�Ƀm�[�h�\�������
	/// </summary>
	void ParseNode(FBXModel* model, FbxNode* fbxNode, Node* parent = nullptr);

	/// <summary>
	/// ���b�V���ǂݎ��
	/// </summary>
	void ParseMesh(FBXModel* fbxModel, FbxNode* fbxNode);

	/// <summary>
	/// ���_���W�ǂݎ��
	/// </summary>
	void ParseVertex(FBXModel* fbxModel, FbxMesh* fbxMesh);

	/// <summary>
	/// �ʏ��ǂݎ��
	/// </summary>
	void ParseFace(FBXModel* fbxModel, FbxMesh* fbxMesh);

	/// <summary>
	/// �}�e���A���ǂݎ��
	/// </summary>
	void ParseMaterial(FBXModel* fbxModel, FbxNode* fbxNode);

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	void LoadTexture(FBXModel* fbxModel, const std::string& fullpath);

private:
	// private�ȃR���X�g���N�^�i�V���O���g���p�^�[���j
	FBXLoader() = default;
	// private�ȃf�X�g���N�^�i�V���O���g���p�^�[���j
	~FBXLoader() = default;
	// �R�s�[�R���X�g���N�^���֎~�i�V���O���g���p�^�[���j
	FBXLoader(const FBXLoader& obj) = delete;
	// �R�s�[������Z�q���֎~�i�V���O���g���p�^�[���j
	void operator=(const FBXLoader& obj) = delete;

private:
	ID3D12Device* device = nullptr;
	FbxManager* fbxManager = nullptr;
	FbxImporter* fbxImporter = nullptr;
};