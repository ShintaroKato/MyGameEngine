#pragma once
#include "ObjectNone.h"
#include "BaseCollider.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"

class Bullet : public ObjectNone
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
	/// <summary>
	/// OBJモデル生成
	/// </summary>
	/// <returns>インスタンス</returns>
	static Bullet* Create();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info) override;

	/// <summary>
	/// 座標を設定
	/// </summary>
	void SetPosition(XMFLOAT3 pos);

	/// <summary>
	/// 回転を設定
	/// </summary>
	void SetRotation(XMFLOAT3 rot);

	/// <summary>
	/// 座標を設定
	/// </summary>
	void SetScale(XMFLOAT3 scale);

	SphereCollider* GetSphereCollider() { return this->sphereColl; };

	// 攻撃力を取得
	float GetPower() { return power; }

public:
	// 座標
	XMFLOAT3 position{};
	// 回転
	XMFLOAT3 rotation{};
	// 半径
	float radius = 1.0f;
	// 速度
	XMVECTOR velocity = { 0, 0, 1.0f };
	// コライダー
	Sphere sphere{};
	SphereCollider* sphereColl = nullptr;
	unsigned short collAttribute = COLLISION_ATTR_BULLET;

	// 攻撃力
	float power = 0.0f;
	// 開始フレーム
	int frame = 0;
	// 終了フレーム
	int end_frame = 100;
};
