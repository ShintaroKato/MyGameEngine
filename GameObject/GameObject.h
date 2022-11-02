#pragma once
#include "Input.h"
#include "ObjectFBX.h"
#include "ObjectOBJ.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include "MeshCollider.h"

class GameObject : public ObjectFBX, public ObjectOBJ
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
	static GameObject* Create(ModelOBJ* obj = nullptr);
	static GameObject* Create(ModelFBX* fbx = nullptr);

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
	/// 敵との当たり判定
	/// </summary>
	bool Hit();

	/// <summary>
	/// オブジェクトを掴む
	/// </summary>
	void Drag();

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 座標を固定された状態にする
	/// </summary>
	void PositionFix()
	{
		isInGame = true;
	}

	BaseCollider* GetCollider() { return meshColl; }

	XMFLOAT3 GetPosition();

	bool GetUsedFlag() { return used; }
	void SetUsedFlag(bool flag)
	{
		if(!isDrag) used = flag;
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
	/// 他のGameObjectと重なるのを防ぐ
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

private:
	// 座標
	XMFLOAT3 pos{};
	// 半径
	float radius = 4.0f;
	// 掴まれているか否か
	bool isDrag = false;
	static bool isDragStatic;
	// 使用されているか否か
	bool used = false;
	// ゲーム本編か否か
	bool isInGame = false;
	// コライダー
	Sphere sphere{};
	MeshCollider* meshColl = nullptr;
	SphereCollider* sphereColl = nullptr;
	// オブジェクトの種類を判別するためのタグ
	std::string tag = "default";

	// 耐久値
	const float HPMax = 20.0f;
	float HP = HPMax;
	float HPRate = HP / HPMax;

	bool aliveFlag = true;
};