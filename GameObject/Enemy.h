#pragma once
#include "CollisionManager.h"
#include "ObjectFBX.h"
#include "ObjectOBJ.h"
#include "SphereCollider.h"
#include "StageObject.h"
#include "Weapon.h"

enum EnemyType
{
	STRAIGHT,
	FLYING,
	ROUTE_RANDOM,
	ROUTE_SEARCH,
	TARGET_PLAYER,
	TYPE_COUNT
};

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
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 移動
	/// </summary>
	void MoveRouteSearch();

	/// <summary>
	/// 飛行
	/// </summary>
	void Fly();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 攻撃
	/// </summary>
	void AttackBeam();

	/// <summary>
	/// 敵との当たり判定
	/// </summary>
	void Hit(float attackPower);

	/// <summary>
	/// 倒された時の処理
	/// </summary>
	void Defeated();

	/// <summary>
	/// 座標を取得
	/// </summary>
	XMFLOAT3 GetPosition();

	/// <summary>
	/// コライダーを取得
	/// </summary>
	BaseCollider* GetCollider() { return sphereColl; }

	// 生存フラグを取得
	bool GetAliveFlag() { return aliveFlag; }

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
	/// 能力値を設定
	/// </summary>
	/// <param name="HP">体力</param>
	/// <param name="speed">速さ(倍率で設定)</param>
	/// <param name="power">攻撃力</param>
	/// <param name="radius">球コライダーの半径の長さ</param>
	void SetStatus(float HP, float speed, float power, float radius);

	/// <summary>
	/// 攻撃対象のオブジェクトを設定
	/// </summary>
	void SetTargetPos(StageObject* object)
	{
		this->target = object;
		SetTargetPos(target->GetPosition());
	}

	/// <summary>
	/// 攻撃対象のオブジェクトの位置を設定
	/// </summary>
	void SetTargetPos(XMFLOAT3 pos)
	{
		targetPos = pos;
		// 標的の方向を向く
		rot.y = XMConvertToDegrees(
			atan2f(targetPos.x - this->pos.x, targetPos.z - this->pos.z));
	}

	/// <summary>
	/// タイプを設定
	/// </summary>
	void SetType(EnemyType enemyType);

	/// <summary>
	/// 武器をセット
	/// </summary>
	void SetWeapon(Weapon* weapon);

	/// <summary>
	/// 武器を取得
	/// </summary>
	Weapon* GetWeapon() { return weapon; }

	/// <summary>
	/// 攻撃力を取得
	/// </summary>
	float GetPower() { return attackPower + weapon->GetPower(); }

	/// <summary>
	/// タイプを取得
	/// </summary>
	EnemyType GetType() { return type; }

private:
	// 座標
	XMFLOAT3 pos{};
	// 回転
	XMFLOAT3 rot{};
	// 半径
	float radius = 1.0f;
	// 移動ベクトル
	XMVECTOR move = { 0,0,0.1f,0 };
	XMVECTOR moveDefault = { 0,0,0.1f,0 };
	// 標的のオブジェクト
	StageObject* target{};
	// 標的の座標
	XMFLOAT3 targetPos{};
	// コライダー
	Sphere sphere{};
	SphereCollider* sphereColl = nullptr;

	// タイプ
	EnemyType type = STRAIGHT;

	// HP
	float HP = 100.0f;
	float HPMax = HP;
	float HPRate = HP / HPMax;

	// 生存フラグ
	bool aliveFlag = false;

	// 無敵時間
	int noDamageTime = 0;
	int noDamageTimeMax = 10;

	// 攻撃
	Weapon* weapon = nullptr;
	bool attackFlag = false;
	float attackPower = 0.1f;

	// 攻撃の間隔
	int attackCountMax = 600.0f;
	int attackCount = attackCountMax;

	bool isStop = false;
};