#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <string>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <vector>
#include <fbxsdk.h>

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

class ModelFBX
{
public:
	// ボーン
	struct Bone
	{
		std::string name;

		DirectX::XMMATRIX invInitialPose;
		FbxCluster* fbxCluster;

		Bone(const std::string& name)
		{
			this->name = name;
		}
	};

	~ModelFBX();

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
	static const int MAX_BONE_INDICES = 4;

private:
	struct VertexPosNormalUvSkin
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
		UINT boneIndex[MAX_BONE_INDICES];
		float boneWeight[MAX_BONE_INDICES];
	};

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
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	// SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	// メッシュノード
	Node* meshNode = nullptr;
	// 頂点配列
	vector<VertexPosNormalUvSkin> vertices;
	// インデックス配列
	vector<unsigned short> indices;
	// ボーン配列
	vector<Bone> bones;
	// FBXシーン
	FbxScene* fbxScene = nullptr;

public:

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// バッファ生成
	/// </summary>
	void CreateBuffers(ID3D12Device* device);

	/// <summary>
	/// モデルの変換行列を取得
	/// </summary>
	const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }

	/// <summary>
	/// ボーンの配列を取得
	/// </summary>
	vector<Bone>& GetBones() { return bones; }

	/// <summary>
	/// FBXシーンを取得
	/// </summary>
	FbxScene* GetFbxScene() { return fbxScene; }
};