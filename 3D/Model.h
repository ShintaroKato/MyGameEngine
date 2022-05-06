#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <vector>
#include <unordered_map>
#include "Mesh.h"

//3D���f��
class Model
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	static const std::string baseDirectory;

public: //�T�u�N���X
	// ���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv;  // uv���W
	};

public: //�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* device;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;

public: //�ÓI�����o�֐�

	// �ÓI������
	static void StaticInitialize(ID3D12Device* device);

	//.obj����3D���f���ǂݍ���
	static Model* LoadFromOBJ(const std::string modelName, bool smoothing = false);

	/// <summary>
	/// �f�X�N���v�^�q�[�v�̏�����
	/// </summary>
	/// <returns>����</returns>
	void InitializeDescriptorHeap();

	static void SetDevice(ID3D12Device* device) { Model::device = device; }

	//���b�V���R���e�i���擾
	inline const std::vector<Mesh*>& GetMesh() { return meshes; }

private: //����J�̃����o�ϐ�
	// ���O
	std::string name;
	//	���b�V���R���e�i
	std::vector<Mesh*> meshes;
	// �}�e���A���R���e�i
	std::unordered_map<std::string, Material*> materials;
	// ���_�f�[�^�z��
	std::vector<VertexPosNormalUv> vertices;
	// ���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;
	// �f�t�H���g�}�e���A��
	Material* defaultMaterial = nullptr;
	// �f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeap;
private: //����J�̃����o�ϐ�

	//.obj����3D���f���ǂݍ��݁i����J�j
	void LoadFromOBJInternal(const std::string modelName, bool smoothing);

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <returns>����</returns>
	bool LoadTexture();

	/// <summary>
	/// �}�e���A���ǂݍ���
	/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// �}�e���A���o�^
	/// </summary>
	void AddMaterial(Material* material);

public:
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	/// <param name="rootParamIndexMaterial">�}�e���A���p���[�g�p�����[�^�ԍ�</param>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial);
};