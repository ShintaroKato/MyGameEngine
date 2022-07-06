#pragma once

#pragma warning (push)
#pragma warning (disable:26495)
//#pragma warning (disable:26812)
#include "fbxsdk.h"
#pragma warning (pop)

#include <d3d12.h>
#include <d3dx12.h>

#include <string>
#include "ModelFBX.h"

class FBXLoader
{
private:// �G�C���A�X
	using string = std::string;

public:
	// ���f���i�[���[�g�p�X
	static const string baseDirectory;
	// �e�N�X�`�����Ȃ��ꍇ�̕W���e�N�X�`���t�@�C����
	static const string defaultTextureFileName;
public:
	/// <summary>
	/// �V���O���g���C���X�^���X�̎擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static FBXLoader* GetInstance();

	/// <summary>
	/// FBX�̍s���XMMatrix�ɕϊ�
	/// </summary>
	/// <param name="dst">�������ݐ�</param>
	/// <param name="src">���ƂȂ�FBX�s��</param>
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src);

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
	ModelFBX* LoadModelFromFile(const string& modelName);

	/// <summary>
	/// �ċA�I�Ƀm�[�h�\�������
	/// </summary>
	void ParseNode(ModelFBX* model, FbxNode* fbxNode, Node* parent = nullptr);

	/// <summary>
	/// ���b�V���ǂݎ��
	/// </summary>
	void ParseMesh(ModelFBX* fbxModel, FbxNode* fbxNode);

	/// <summary>
	/// ���_���W�ǂݎ��
	/// </summary>
	void ParseVertex(ModelFBX* fbxModel, FbxMesh* fbxMesh);

	/// <summary>
	/// �ʏ��ǂݎ��
	/// </summary>
	void ParseFace(ModelFBX* fbxModel, FbxMesh* fbxMesh);

	/// <summary>
	/// �}�e���A���ǂݎ��
	/// </summary>
	void ParseMaterial(ModelFBX* fbxModel, FbxNode* fbxNode);

	/// <summary>
	/// �X�L�j���O���ǂݎ��
	/// </summary>
	void ParseSkin(ModelFBX* model, FbxMesh* fbxMesh);

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	void LoadTexture(ModelFBX* fbxModel, const std::string& fullpath);

	/// <summary>
	/// �f�B���N�g�����܂񂾃t�@�C���p�X����t�@�C�����𒊏o
	/// </summary>
	std::string ExtractFileName(const std::string& path);

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