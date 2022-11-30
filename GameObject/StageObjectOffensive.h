#pragma once
#include "Input.h"
#include "ObjectFBX.h"
#include "ObjectOBJ.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include "MeshCollider.h"

class StageObjectOffensive : public ObjectFBX, public ObjectOBJ
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
	static StageObjectOffensive* Create(ModelOBJ* obj = nullptr);
	static StageObjectOffensive* Create(ModelFBX* fbx = nullptr);

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
	/// 敵との当たり判定
	/// </summary>
	void Hit(float attackPower);

	/// <summary>
	/// 座標を固定された状態にする
	/// </summary>
	void PositionFix()
	{
		isInGame = true;
	}

	BaseCollider* GetCollider() { return meshColl; }

	XMFLOAT3 GetPosition();

	bool GetUsedState() { return used; }
	void SetUsedState(bool flag)
	{
		used = flag;
		SetPosition(pos);
	}

	void SetPosition(const XMFLOAT3& position);

	void SetRadius(float radius) { this->radius = radius; }

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
	/// タグを取得
	/// </summary>
	std::string GetTag() { return tag; }

	/// <summary>
	/// タグをセット
	/// </summary>
	void SetTag(std::string tag) { this->tag = tag; }

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

private:
	// 座標
	XMFLOAT3 pos{};
	XMFLOAT3 posTmp{};
	// 半径
	float radius = 4.0f;
	// 使用されているか否か
	bool used = false;
	// ゲーム本編か否か
	bool isInGame = false;
	// コライダー
	Sphere sphere{};
	MeshCollider* meshColl = nullptr;
	// オブジェクトの種類を判別するためのタグ
	std::string tag = "default";

	// 衝突しているか否か
	bool hit = false;

	// 耐久値
	float HPMax = 100.0f;
	float HP = HPMax;
	float HPRate = HP / HPMax;

	bool aliveFlag = true;
};