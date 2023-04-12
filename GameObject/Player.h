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
	/// 壁からの押し出し
	/// </summary>
	void Rejection();

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

	/// <summary>
	/// ゲーム本編か否かを設定
	/// </summary>
	void SetInGameFlag(bool flag)
	{
		isInGame = flag;
		cameraRotY = 0;
		cameraRotX = 30;
	}

	/// <summary>
	/// カメラの移動が有効か否かを設定
	/// </summary>
	void SetCameraMoveFlag(bool flag) { cameraMoveActive = flag; }

	/// <summary>
	/// カメラの操作ができる状態か否かを設定
	/// </summary>
	void SetCameraControlFlag(bool flag) { cameraControlActive = flag; }

	/// <summary>
	/// カメラの操作ができる状態か否かを取得
	/// </summary>
	bool GetCameraControlFlag() { return cameraControlActive; }

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


private:
	// 座標
	XMFLOAT3 pos{};
	// 回転
	XMFLOAT3 rot{};
	// 半径
	float radius = 1.0f;
	// 接地フラグ
	bool onGround = true;
	// 押し出しフラグ
	bool rejectX = false;
	bool rejectZ = false;
	// 移動ベクトル(初期値)
	XMVECTOR move_default = { 0,0,0.3f,0 };
	// 移動ベクトル
	XMVECTOR move{};
	// 攻撃時の移動速度
	float attackMove = 0.4f;
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
	bool cameraMoveActive = true;
	bool cameraControlActive = true;
	// カメラと地形との衝突判定をとるコライダー
	Sphere sphereCamera{};
	SphereCollider* sphereCameraColl = nullptr;

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
};