#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include "PipelineSet.h"
#include "ModelOBJ.h"
#include "Camera.h"
#include "CollisionInfo.h"

class BaseCollider;

/// <summary>
/// 3Dオブジェクト
/// </summary>
class ObjectOBJ
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス

	// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		XMMATRIX viewProj;	// ビュープロジェクション行列
		XMMATRIX world;				// ワールド行列
		XMFLOAT3 camPos;			// カメラの位置
		UINT mode;			// シェーディングのモード
		XMFLOAT4 constColor;				// 色
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
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static ObjectOBJ* Create();

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
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

public:
	/// <summary>
	/// カメラの設定
	/// </summary>
	/// <param name="camera"></param>
	static void SetCamera(Camera* camera) { ObjectOBJ::camera = camera; }

	/// <summary>
	/// デバイスの設定
	/// </summary>
	/// <param name="device"></param>
	static void SetDevice(ID3D12Device* device) { ObjectOBJ::device = device; }

public: // メンバ関数
	virtual bool Initialize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// ワールド行列の更新
	/// </summary>
	void UpdateWorldMatrix();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3& GetPosition() { return position; }

	/// <summary>
	/// 回転の取得
	/// </summary>
	/// <returns></returns>
	const XMFLOAT3& GetRotation() { return rotation; }

	/// <summary>
	/// ワールド行列の取得
	/// </summary>
	/// <returns>ワールド行列</returns>
	const XMMATRIX& GetMatWorld() { return matWorld; }
	const XMMATRIX& GetInvMatWorld() { return invMatWorld; }

	/// <summary>
	/// カメラからの距離を取得
	/// </summary>
	const float& GetCameraDistance() { return cameraDistance; }

	/// <summary>
	/// コライダーを取得
	/// </summary>
	BaseCollider* GetCollider() { return collider; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const XMFLOAT3& position) { this->position = position; }

	/// <summary>
	/// 回転の設定
	/// </summary>
	/// <param name="rotation">回転</param>
	void SetRotation(const XMFLOAT3& rotation) { this->rotation = rotation; }

	/// <summary>
	/// スケールの設定
	/// </summary>
	/// <param name="scale">スケール</param>
	void SetScale(const XMFLOAT3& scale) { this->scale = scale; }

	/// <summary>
	/// 色の設定
	/// </summary>
	/// <param name="color">色(RGBA)</param>
	void SetColor(const XMFLOAT4& color) { this->color = color; }

	/// <summary>
	/// モデルの設定
	/// </summary>
	void SetModelOBJ(ModelOBJ* model) { this->model = model; }

	/// <summary>
	/// コライダーの設定
	/// </summary>
	void SetCollider(BaseCollider* collider);

	/// <summary>
	/// シェーディングのモードを設定
	/// </summary>
	/// <param name="mode"></param>
	void SetShadingMode(UINT mode) { shadeMode = mode; }

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	virtual void OnCollision(const CollisionInfo& info) {}

private: // メンバ変数
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuffB0;

protected: // メンバ変数
	// モデル
	ModelOBJ* model = nullptr;
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
	XMMATRIX invMatWorld{};
	// 親オブジェクト
	ObjectOBJ* parent = nullptr;
	// カメラからの距離
	float cameraDistance = 0;
	// コライダー
	BaseCollider* collider = nullptr;

	// シェーディングのモード
	UINT shadeMode = 0;

protected:
	Camera* GetCamera() { return ObjectOBJ::camera; }
};