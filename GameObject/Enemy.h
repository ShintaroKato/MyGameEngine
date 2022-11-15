#pragma once
#include "CollisionManager.h"
#include "ObjectFBX.h"
#include "ObjectOBJ.h"
#include "SphereCollider.h"
#include "GameObject.h"
#include "GameManager.h"

class Enemy : public ObjectFBX, public ObjectOBJ
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
	static Enemy* Create(ModelOBJ* obj = nullptr);

	/// <summary>
	/// FBXモデル生成
	/// </summary>
	/// <param name="fbx">モデルデータ</param>
	/// <param name="animationNumber">アニメーション番号</param>
	/// <returns>インスタンス</returns>
	static Enemy* Create(ModelFBX* fbx = nullptr, int animationNumber = 0);

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
	/// 出現
	/// </summary>
	void Spawn();

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 落下・ジャンプ
	/// </summary>
	void Jump();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 敵との当たり判定
	/// </summary>
	void Hit(float attackPower);

	/// <summary>
	/// 座標を取得
	/// </summary>
	XMFLOAT3 GetPosition();

	/// <summary>
	/// コライダーを取得
	/// </summary>
	BaseCollider* GetCollider() { return sphereColl; }

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

	/// <summary>
	/// 生存フラグを設定
	/// </summary>
	void SetAllive(bool flag) { this->aliveFlag = flag; }

	/// <summary>
	/// 攻撃対象の座標を設定
	/// </summary>
	void SetTargetPos(XMFLOAT3 pos) { this->target = pos; }

	/// <summary>
	/// ゲーム内であるか否かを設定
	/// </summary>
	void SetInGame(bool inGame)
	{
		this->isInGame = inGame;
		if (!isInGame) aliveFlag = false;
	}

private:
	// 座標
	XMFLOAT3 pos{};
	// 回転
	XMFLOAT3 rotation{};
	// 半径
	float radius = 1.0f;
	// 移動ベクトル
	XMVECTOR move = { 0,0,0.1f,0 };
	// 標的の座標
	XMFLOAT3 target{};
	// コライダー
	Sphere sphere{};
	SphereCollider* sphereColl = nullptr;

	// HP
	float HP = 20.0f;
	const float HPMax = HP;
	float HPRate = HP / HPMax;

	bool aliveFlag = false;

	// 攻撃
	bool attackFlag = false;

	// ゲーム内であるか否か
	bool isInGame = false;
};