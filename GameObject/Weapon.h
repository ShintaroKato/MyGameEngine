#pragma once
#include "CollisionManager.h"
#include "ObjectFBX.h"
#include "ObjectOBJ.h"
#include "SphereCollider.h"

class Weapon : public ObjectFBX, public ObjectOBJ
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
	static Weapon* Create(ModelOBJ* obj = nullptr);

	/// <summary>
	/// FBXモデル生成
	/// </summary>
	/// <param name="fbx">モデルデータ</param>
	/// <param name="animationNumber">アニメーション番号</param>
	/// <returns>インスタンス</returns>
	static Weapon* Create(ModelFBX* fbx = nullptr, int animationNumber = 0);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize() override;

	/// <summary>
	/// 親オブジェクトセット
	/// </summary>
	void SetParent(ObjectOBJ* obj);
	void SetParent(ObjectFBX* fbx);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info) override;

	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	XMFLOAT3 GetPosition() { return pos; }

	/// <summary>
	/// 回転を取得
	/// </summary>
	/// <returns>回転</returns>
	XMFLOAT3 GetRotation() { return rot; }

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
	float GetPower(){ return power; }

private:
	// 座標
	XMFLOAT3 pos{};
	// 回転
	XMFLOAT3 rot{};
	// 半径
	float radius = 2.0f;
	// 接地フラグ
	bool onGround = true;
	// 親オブジェクト
	ObjectOBJ* obj = nullptr;
	ObjectFBX* fbx = nullptr;
	// コライダー
	Sphere sphere{};
	SphereCollider* sphereColl = nullptr;

	// 攻撃力
	float power = 100.0f;
};
