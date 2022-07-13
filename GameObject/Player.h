#pragma once
#include "ObjectFBX.h"
#include "ObjectOBJ.h"

class Player : public ObjectFBX, public ObjectOBJ
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
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns>インスタンス</returns>
	static Player* Create(ModelOBJ* obj = nullptr);
	static Player* Create(ModelFBX* fbx = nullptr);

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize() override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 移動・ジャンプ
	/// </summary>
	void Move();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 敵との当たり判定
	/// </summary>
	bool Hit();

	//BaseCollider* GetCollider() { return collider; }

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	//void OnCollision(const CollisionInfo& info) override;

private:

	// HP
	float HP = 20.0f;
	const float HPMax = HP;
	float HPRate = HP / HPMax;

	bool alliveFlag = true;

	// 攻撃
	bool attackFlag = false;
};