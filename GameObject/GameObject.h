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
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 敵との当たり判定
	/// </summary>
	bool Hit();

	/// <summary>
	/// オブジェクトを掴む
	/// </summary>
	void Drag();

	/// <summary>
	/// 座標を固定された状態にする
	/// </summary>
	void PositionFix() { fix = true; }

	BaseCollider* GetCollider() { return meshColl; }

	XMFLOAT3 GetPosition();

	void SetPosition(const XMFLOAT3& pos);

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info) override;

private:
	// 座標
	XMFLOAT3 pos{};
	// 半径
	float radius = 5.0f;
	// 掴まれているか否か
	bool isDrag = false;
	// 位置固定
	bool fix = false;
	// コライダー
	Sphere sphere{};
	MeshCollider* meshColl = nullptr;
};