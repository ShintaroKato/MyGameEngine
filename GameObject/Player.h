#pragma once
#include "CollisionManager.h"
#include "ObjectFBX.h"
#include "ObjectOBJ.h"
#include "SphereCollider.h"
#include "GameObject.h"
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
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 落下・ジャンプ
	/// </summary>
	void Jump();

	/// <summary>
	/// 視点操作
	/// </summary>
	void ControlCamera();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 敵との当たり判定
	/// </summary>
	bool Hit();

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

	void Rejection(const CollisionInfo& info);

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
	/// プレイヤーとカメラの距離
	/// </summary>
	void SetCameraDistance(float distance) { this->distance = distance; }

	/// <summary>
	/// 生存フラグを設定
	/// </summary>
	void SetAllive(bool flag) { this->aliveFlag = flag; }

	///// <summary>
	///// 武器をセット
	///// </summary>
	void SetWeapon(Weapon* weapon);

	/// <summary>
	/// ゲーム本編か否かを設定
	/// </summary>
	void SetInGameFlag(bool flag)
	{
		isInGame = flag;
		cameraRotY = 0;
		cameraRotX = 30;
	}

	void SetCameraMoveFlag(bool flag) { cameraMoveOn = flag; }

	Weapon* GetWeapon() { return weapon; }

	float GetHP() { return HP; }
	float GetHPMax()
	{
		float max = HPMax;
		return max;
	}

	float GetPower() { return attackPower + weapon->GetPower(); }

private:
	// 座標
	XMFLOAT3 pos{};
	// 回転
	XMFLOAT3 rotation{};
	// 半径
	float radius = 1.0f;
	// 接地フラグ
	bool onGround = true;
	// 移動ベクトル
	XMVECTOR move = { 0,0,1.0f,0 };
	// 落下ベクトル
	DirectX::XMVECTOR fallVel;
	// コライダー
	Sphere sphere{};
	SphereCollider* sphereColl = nullptr;

	// カメラ
	XMFLOAT3 cameraPos{};
	XMFLOAT3 cameraRot{};
	float cameraRotX = 0;
	float cameraRotY = 0;
	float distance = 10;
	// カメラ操作ON・OFF
	bool cameraMoveOn = true;

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
	float attackPower = 10.0f;

	// ゲーム本編か否か
	bool isInGame = true;
};