#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>

class Camera
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	/// <summary>
	/// カメラ初期化
	/// </summary>
	/// <param name="window_width">画面横幅</param>
	/// <param name="window_height">画面縦幅</param>
	static void Initialize(const int& window_width, const int& window_height);

	/// <summary>
	/// 更新
	/// </summary>
	static void Update();

	/// <summary>
	/// ビュー行列を更新
	/// </summary>
	static void UpdateViewMatrix();

	/// <summary>
	/// 射影行列を更新
	/// </summary>
	static void UpdateProjectionMatrix();

	/// <summary>
	/// 視点座標の取得
	/// </summary>
	/// <returns>座標</returns>
	static const XMFLOAT3& GetEye() { return eye; }

	/// <summary>
	/// ビュー行列取得
	/// </summary>
	static const XMMATRIX& GetViewMatrix() { return matView; }

	/// <summary>
	/// 射影行列取得
	/// </summary>
	static const XMMATRIX& GetProjectionMatrix() { return matProjection; }
	
	/// <summary>
	/// 射影行列取得
	/// </summary>
	static const XMMATRIX& GetViewProjectionMatrix() { return matView * matProjection; }

	/// <summary>
	/// ビルボード行列取得
	/// </summary>
	static const XMMATRIX& GetBillboardMatrix() { return matBillboard; }

	/// <summary>
	/// 視点座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	static void SetEye(XMFLOAT3 eye);

	/// <summary>
	/// 注視点座標の取得
	/// </summary>
	/// <returns>座標</returns>
	static const XMFLOAT3& GetTarget() { return target; }

	/// <summary>
	/// 注視点座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	static void SetTarget(XMFLOAT3 target);

	/// <summary>
	/// ベクトルによる移動
	/// </summary>
	/// <param name="move">移動量</param>
	static void CameraMoveVector(XMFLOAT3 move);


private:
	// 視点座標
	static XMFLOAT3 eye;
	// 注視点座標
	static XMFLOAT3 target;
	// 上方向ベクトル
	static XMFLOAT3 up;
	// ビュー行列
	static XMMATRIX matView;
	// 射影行列
	static XMMATRIX matProjection;
	// ビュー射影行列
	static XMMATRIX matViewProjection;
	// ビルボード行列
	static XMMATRIX matBillboard;
	// アスペクト比
	static float aspect;
};