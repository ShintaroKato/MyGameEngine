#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include "PipelineSet.h"
#include "FBXLoader.h"
#include "Camera.h"

/// <summary>
/// 3Dオブジェクト
/// </summary>
class ObjectFBX
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 定数
	// ボーンの最大数
	static const int MAX_BONES = 32;

public: // サブクラス

	// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		XMMATRIX viewProj;	// ビュープロジェクション行列
		XMMATRIX world;				// ワールド行列
		XMFLOAT3 camPos;			// カメラの位置
	};

	// 定数バッファ用データ構造体(スキニング)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	/// <returns>成否</returns>
	static bool StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static ObjectFBX* Create();
	/// <summary>
	/// カメラの設定
	/// </summary>
	/// <param name="camera"></param>
	static void SetCamera(Camera* camera) { ObjectFBX::camera = camera; }
	/// <summary>
	/// デバイスの設定
	/// </summary>
	/// <param name="device"></param>
	static void SetDevice(ID3D12Device* device) { ObjectFBX::device = device; }

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// パイプラインセット
	static PipelineSet pipelineSet;
	// カメラ
	static Camera* camera;

private:// 静的メンバ関数

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static bool InitializeGraphicsPipeline();

public: // メンバ関数
	virtual bool Initialize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3& GetPosition() { return position; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(XMFLOAT3 position) { this->position = position; }

	/// <summary>
	/// 回転の設定
	/// </summary>
	/// <param name="rotation">回転</param>
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }

	/// <summary>
	/// スケールの設定
	/// </summary>
	/// <param name="scale">スケール</param>
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	/// <summary>
	/// モデルの設定
	/// </summary>
	void SetModel(ModelFBX* model) { this->model = model; }

	/// <summary>
	/// アニメーションセット
	/// </summary>
	/// <param name="number">アニメーション番号</param>
	void SetAnimationNumber(int number);

	/// <summary>
	/// アニメーション再生
	/// </summary>
	void AnimationPlay();

	/// <summary>
	/// アニメーション一時停止
	/// </summary>
	void AnimationStop();

	/// <summary>
	/// アニメーションをリセットして停止
	/// </summary>
	void AnimationReset();

private: // メンバ変数
	// モデル
	ModelFBX* model = nullptr;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuffB0;
	// 定数バッファ(スキン)
	ComPtr<ID3D12Resource> constBufferSkin;
	// 色
	XMFLOAT4 color = { 1,1,1,1 };
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld{};
	// 親オブジェクト
	ObjectFBX* parent = nullptr;
	// アニメーション時間情報
	FbxTakeInfo* takeInfo = nullptr;
	// 1フレームの時間
	FbxTime frameTime;
	// アニメーション開始時間
	FbxTime startTime;
	// アニメーション終了時間
	FbxTime endTime;
	// 現在時間
	FbxTime currentTime;
	// アニメーション再生中
	bool isPlay = false;
};