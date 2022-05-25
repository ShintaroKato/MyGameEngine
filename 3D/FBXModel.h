#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <string>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <vector>

// �m�[�h
struct Node
{
	std::string name;

	DirectX::XMVECTOR scaling = { 1,1,1,0 };
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	DirectX::XMVECTOR translation = { 0,0,0,1 };
	DirectX::XMMATRIX transform;
	DirectX::XMMATRIX globalTransform;

	Node* parent = nullptr;
};

class FBXModel
{
private:
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;

	using string = std::string;
	template <class T> using vector = std::vector<T>;

public:
	friend class FBXLoader;

public:
	struct VertexPosNormalUv
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
	};

	Node* meshNode = nullptr;
	std::vector<VertexPosNormalUv> vertices;
	std::vector<unsigned short> indices;

private:
	// ���f����
	string name;
	// �m�[�h�z��
	vector<Node> node;
	// �A���r�G���g�W��
	XMFLOAT3 ambient = { 1,1,1 };
	// �f�B�t���[�Y�W��
	XMFLOAT3 diffuse = { 1,1,1 };
	// �e�N�X�`�����^�f�[�^
	TexMetadata metadata = {};
	// �X�N���b�`�C���[�W
	ScratchImage scratchImg = {};
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// �C���f�b�N�X�o�b�t�@�r���[
};