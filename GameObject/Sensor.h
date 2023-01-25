#pragma once
#include "Input.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include "MeshCollider.h"

class Sensor : public BaseCollider
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

public:

	void Initialize();

	// 敵を検知する範囲を設定
	void SetSensorRange(float range);

	// 敵を検知
	void Detection();

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info);

private:

	// 敵を検知する範囲(球の半径)
	Sphere sensorSphere{};
	SphereCollider* sensorColl = nullptr;

	// 標的の座標
	XMFLOAT3 targetPos{};

	// 検知している否か
	bool isDetecting = false;

	// 衝突しているか否か
	bool hit = false;

	bool aliveFlag = true;
};