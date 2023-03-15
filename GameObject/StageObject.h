#pragma once
#include "Input.h"
#include "ObjectFBX.h"
#include "ObjectOBJ.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include "MeshCollider.h"
#include "Sensor.h"

enum UsedState
{
	UNUSED,
	WAITING,
	USED
};

enum Tag
{
	STAGE_OBJECT_DEFAULT,
	RED_OBJECT,
	GREEN_OBJECT,
	BLUE_OBJECT,
	CASTLE_OBJECT,
	OFFENCE_OBJECT,
};

class StageObject : public ObjectFBX, public ObjectOBJ
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
	static StageObject* Create(ModelOBJ* obj = nullptr);
	static StageObject* Create(ModelFBX* fbx = nullptr);

public:
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns>インスタンス</returns>
	StageObject* GetInstance() { return this; };
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
	/// 敵との当たり判定
	/// </summary>
	void Hit(float attackPower);

	/// <summary>
	/// オブジェクトを掴む
	/// </summary>
	void Drag();

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 回転
	/// </summary>
	void Rotation();

	/// <summary>
	/// デフォルトの状態に変更
	/// </summary>
	void ChangeDefault();

	/// <summary>
	/// オブジェクトのタイプ
	/// </summary>
	void ObjectType();

	/// <summary>
	/// 弾を発射
	/// </summary>
	void Attack();

	/// <summary>
	/// ゲーム本編か否かを設定
	/// </summary>
	void SetInGameFlag(bool isInGame)
	{
		this->isInGame = isInGame;
	}

	BaseCollider* GetCollider() { return meshColl; }

	XMFLOAT3 GetPosition();

	void SetPosition(const XMFLOAT3& position);

	UsedState GetUsedState() { return used; }
	void SetUsedState(UsedState state)
	{
		if(!isDrag) used = state;
		SetPosition(pos);
	}

	void SetRadius(float radius) { this->radius = radius; }
	void SetSquareSideLength(float length_X, float length_Y)
	{
		this->sphereColl->SetSquareSideLength(length_X, length_Y);
	}

	void SetModel(ModelOBJ* obj = nullptr);
	void SetModel(ModelFBX* fbx = nullptr);

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info) override;

	/// <summary>
	/// 他のStageObjectと重なるのを防ぐ
	/// </summary>
	void Rejection(const CollisionInfo& info);

	/// <summary>
	/// 掴まれているか否かの判定
	/// </summary>
	bool GetDragFlag() { return isDrag; }

	void ChangeDragFlagFalse()
	{
		isDrag = false;
		isDragStatic = false;
	}

	/// <summary>
	/// タグを取得
	/// </summary>
	Tag GetTag() { return tag; }

	/// <summary>
	/// タグをセット
	/// </summary>
	void SetTag(Tag tag)
	{
		this->tag = tag;
		ObjectType();
	}

	int GetNumber() { return number; }

	void SetNumber(int number) { this->number = number; }

	void SetAlive(bool aliveFlag) { this->aliveFlag = aliveFlag; }

	/// <summary>
	/// 耐久値を設定
	/// </summary>
	/// <returns></returns>
	void SetHP(float HP)
	{
		HPMax = HP;
		this->HP = HPMax;
	}

	/// <summary>
	/// 現在の耐久値を取得
	/// </summary>
	/// <returns></returns>
	float GetHP() { return HP; }

	/// <summary>
	/// 耐久値の最大値を取得
	/// </summary>
	/// <returns></returns>
	float GetHPMax() { return HPMax; }

	void ResetStatus()
	{
		HP = HPMax;
		aliveFlag = true;
	}

protected:
	// 座標
	XMFLOAT3 pos{};
	XMFLOAT3 rot{};
	// 半径
	float radius = 3.5f;
	// 掴まれているか否か
	bool isDrag = false;
	static bool isDragStatic;
	// 使用されているかを判別
	UsedState used = UNUSED;
	// ゲーム本編か否か
	bool isInGame = false;
	// コライダー
	Sphere sphere{};
	MeshCollider* meshColl = nullptr;
	SphereCollider* sphereColl = nullptr;
	// オブジェクトの種類を判別するためのタグ
	Tag tag = STAGE_OBJECT_DEFAULT;
	// 番号
	int number = -1;

	// 衝突しているか否か
	bool hit = false;

	// 攻撃力
	float power = 100.0f;
	// 攻撃の間隔
	int attackWaitCountMax = 100.0f;
	int attackWaitCount = attackWaitCountMax;
	// 敵を検知するセンサー
	Sensor* sensor{};

	// 耐久値
	float HPMax = 100.0f;
	float HP = HPMax;
	float HPRate = HP / HPMax;

	bool aliveFlag = true;
};

bool operator<(StageObject l, StageObject r);