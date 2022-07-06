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
private:// エイリアス
	using string = std::string;

public:
	// モデル格納ルートパス
	static const string baseDirectory;
	// テクスチャがない場合の標準テクスチャファイル名
	static const string defaultTextureFileName;
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FBXLoader* GetInstance();

	/// <summary>
	/// FBXの行列をXMMatrixに変換
	/// </summary>
	/// <param name="dst">書き込み先</param>
	/// <param name="src">元となるFBX行列</param>
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	void Initialize(ID3D12Device* device);

	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize();

	/// <summary>
	/// ファイルからFBXモデル読み込み
	/// </summary>
	/// <param name="modelName">モデル名</param>
	ModelFBX* LoadModelFromFile(const string& modelName);

	/// <summary>
	/// 再帰的にノード構造を解析
	/// </summary>
	void ParseNode(ModelFBX* model, FbxNode* fbxNode, Node* parent = nullptr);

	/// <summary>
	/// メッシュ読み取り
	/// </summary>
	void ParseMesh(ModelFBX* fbxModel, FbxNode* fbxNode);

	/// <summary>
	/// 頂点座標読み取り
	/// </summary>
	void ParseVertex(ModelFBX* fbxModel, FbxMesh* fbxMesh);

	/// <summary>
	/// 面情報読み取り
	/// </summary>
	void ParseFace(ModelFBX* fbxModel, FbxMesh* fbxMesh);

	/// <summary>
	/// マテリアル読み取り
	/// </summary>
	void ParseMaterial(ModelFBX* fbxModel, FbxNode* fbxNode);

	/// <summary>
	/// スキニング情報読み取り
	/// </summary>
	void ParseSkin(ModelFBX* model, FbxMesh* fbxMesh);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	void LoadTexture(ModelFBX* fbxModel, const std::string& fullpath);

	/// <summary>
	/// ディレクトリを含んだファイルパスからファイル名を抽出
	/// </summary>
	std::string ExtractFileName(const std::string& path);

private:
	// privateなコンストラクタ（シングルトンパターン）
	FBXLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FBXLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FBXLoader(const FBXLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FBXLoader& obj) = delete;

private:
	ID3D12Device* device = nullptr;
	FbxManager* fbxManager = nullptr;
	FbxImporter* fbxImporter = nullptr;
};