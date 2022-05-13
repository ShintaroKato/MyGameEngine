#pragma once

#include "fbxsdk.h"

#include <d3d12.h>
#include <d3dx12.h>

#include <string>

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