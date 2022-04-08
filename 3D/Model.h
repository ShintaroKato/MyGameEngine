#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <vector>
#include <unordered_map>

//3Dモデル
class Model
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: //サブクラス
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};

	// マテリアル
	struct Material
	{
		std::string name;	// マテリアル名
		XMFLOAT3 ambient;	// アンビエント影響度
		XMFLOAT3 diffuse;	// ディフューズ影響度
		XMFLOAT3 specular;	// スペキュラー影響度
		float alpha;		// アルファ
		std::string textureFilename;	// テクスチャファイル名
		// コンストラクタ
		Material() {
			ambient = { 0.3f, 0.3f, 0.3f };
			diffuse = { 0.0f, 0.0f, 0.0f };
			specular = { 0.0f, 0.0f, 0.0f };
			alpha = 1.0f;
		}
	};

	// 定数バッファ用データ構造体B1
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient; // アンビエント係数
		float pad1; // パディング
		XMFLOAT3 diffuse; // ディフューズ係数
		float pad2; // パディング
		XMFLOAT3 specular; // スペキュラー係数
		float alpha;	// アルファ
	};

public: //静的メンバ変数
	//デバイス
	static ID3D12Device* device;
	// マテリアル
	static Material material;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	static ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	static ComPtr<ID3D12Resource> indexBuff;
	// テクスチャバッファ
	static ComPtr<ID3D12Resource> texbuff;
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// 頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// インデックスバッファビュー
	static D3D12_INDEX_BUFFER_VIEW ibView;

public: //静的メンバ関数

	//.objから3Dモデル読み込み
	static Model* LoadFromOBJ(const std::string modelName);

	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	/// <returns>成否</returns>
	static void InitializeDescriptorHeap();

	static void SetDevice(ID3D12Device* device) { Model::device = device; }

private: //非公開のメンバ変数
	// 頂点データ配列
	std::vector<VertexPosNormalUv> vertices;
	// 頂点インデックス配列
	std::vector<unsigned short> indices;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuffB1;

private: //非公開のメンバ変数

	//.objから3Dモデル読み込み（非公開）
	void LoadFromOBJInternal(const std::string modelName);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <returns>成否</returns>
	bool LoadTexture(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// 各種バッファ生成
	/// </summary>
	void CreateBuffers();

public:
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	/// <param name="rootParamIndexMaterial">マテリアル用ルートパラメータ番号</param>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial);
};