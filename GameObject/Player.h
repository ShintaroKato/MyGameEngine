#pragma once
#include "CollisionManager.h"
#include "ObjectFBX.h"
#include "ObjectOBJ.h"
#include "SphereCollider.h"
#include "StageObject.h"
#include "Weapon.h"

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

	enum JumpState
	{
		STAY_IN_AIR,
		ON_GROUND,
		WALL_JUMP_BEFORE,
		WALL_JUMP_AFTER,
	};

public:
	/// <summary>
	/// OBJモデル生成
	/// </summary>
	/// <returns>インスタンス</returns>
	static Player* Create(ModelOBJ* obj = nullptr);

	/// <summary>
	/// FBXモデル生成
	/// </summary>
	/// <param name="fbx">モデルデータ</param>
	/// <param name="animationNumber">アニメーション番号</param>
	/// <returns>インスタンス</returns>
	static Player* Create(ModelFBX* fbx = nullptr, int animationNumber = 0);

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

private:
	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 落下・ジャンプ
	/// </summary>
	void Jump();

	/// <summary>
	/// 横方向にステップ
	/// </summary>
	void Step();

	/// <summary>
	/// 壁からの押し出し
	/// </summary>
	void Rejection();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 攻撃エフェクト
	/// </summary>
	void AttackEffect();

	/// <summary>
	/// 敵との当たり判定
	/// </summary>
	bool Hit();

public:
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

	void SetCamera(Camera* camera)
	{
		this->camera = camera;
		if (ObjectOBJ::model) ObjectOBJ::SetCamera(camera);
		if (ObjectFBX::model) ObjectFBX::SetCamera(camera);
	}

	/// <summary>
	/// 生存フラグを設定
	/// </summary>
	void SetAlive(bool flag) { this->aliveFlag = flag; }

	/// <summary>
	/// ゲーム本編か否かを設定
	/// </summary>
	void SetInGameFlag(bool flag)
	{
		isInGame = flag;
	}

	///// <summary>
	///// 武器をセット
	///// </summary>
	void SetWeapon(Weapon* weapon);

	/// <summary>
	/// 武器を取得
	/// </summary>
	Weapon* GetWeapon() { return weapon; }

	/// <summary>
	/// 攻撃力を取得
	/// </summary>
	float GetPower() { return power + weapon->GetPower(); }

	float GetHP() { return HP; }
	float GetHPMax()
	{
		float max = HPMax;
		return max;
	}

	/// <summary>
	/// 出現時の演出
	/// </summary>
	bool SpawnAnimation();

	void AnimationTimerReset() { animTimer = 0; }

private:
	// 座標
	XMFLOAT3 pos{};
	// 回転
	XMFLOAT3 rot{};
	// 半径
	float radius = 1.0f;
	// ジャンプ状態
	JumpState jumpState = ON_GROUND;
	// 押し出しフラグ
	bool rejectX = false;
	bool rejectZ = false;
	// 移動ベクトル(初期値)
	XMVECTOR move_default = { 0,0,0.3f,0 };
	// 移動ベクトル
	XMVECTOR move = move_default;
	// 攻撃時の移動速度
	float attackMove = 0.4f;
	// 落下ベクトル
	DirectX::XMVECTOR fallVel;
	// ステップをしたか
	bool isStepped = false;
	int stepTimerMax = 30;
	int stepTimer = stepTimerMax;
	int stepEnd = 10;
	XMFLOAT3 stepStartPos{};
	XMFLOAT3 stepEndPos{};
	// 壁に当たっているか否か
	bool hitWall = false;

	// コライダー
	Sphere sphere{};
	SphereCollider* sphereColl = nullptr;

	// カメラ
	Camera* camera = nullptr;

	// HP
	const float HPMax = 200.0f;
	float HP = HPMax;
	float HPRate = HP / HPMax;

	bool aliveFlag = true;

	// 攻撃
	Weapon* weapon = nullptr;
	bool attackFlag = false;
	int attackLevel = 0;
	float attackCount = 0;
	float power = 10.0f;

	// ゲーム本編か否か
	bool isInGame = true;

	// 出現時のアニメーション用タイマー
	float animTimer = 0;
	float animTimerMax = 120;
	bool isAnimated = false;
};