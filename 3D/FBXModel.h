#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <string>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <vector>

// ノード
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
	// モデル名
	string name;
	// ノード配列
	vector<Node> node;
	// アンビエント係数
	XMFLOAT3 ambient = { 1,1,1 };
	// ディフューズ係数
	XMFLOAT3 diffuse = { 1,1,1 };
	// テクスチャメタデータ
	TexMetadata metadata = {};
	// スクラッチイメージ
	ScratchImage scratchImg = {};
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// インデックスバッファビュー
};