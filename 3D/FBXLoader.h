#pragma once

#include "fbxsdk.h"

#include <d3d12.h>
#include <d3dx12.h>

#include <string>
#include "FBXModel.h"

class FBXLoader
{
private:// エイリアス
	using string = std::string;

public:
	// モデル格納ルートパス
	static const string baseDirectory;

public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FBXLoader* GetInstance();

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
	void LoadModelFromFile(const string& modelName);

	/// <summary>
	/// 再帰的にノード構造を解析
	/// </summary>
	void ParseNode(FBXModel* model, FbxNode* fbxNode, Node* parent = nullptr);

	/// <summary>
	/// メッシュ読み取り
	/// </summary>
	void ParseMesh(FBXModel* fbxModel, FbxNode* fbxNode);

	/// <summary>
	/// 頂点座標読み取り
	/// </summary>
	void ParseVertex(FBXModel* fbxModel, FbxMesh* fbxMesh);

	/// <summary>
	/// 面情報読み取り
	/// </summary>
	void ParseFace(FBXModel* fbxModel, FbxMesh* fbxMesh);

	/// <summary>
	/// マテリアル読み取り
	/// </summary>
	void ParseMaterial(FBXModel* fbxModel, FbxNode* fbxNode);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	void LoadTexture(FBXModel* fbxModel, const std::string& fullpath);

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