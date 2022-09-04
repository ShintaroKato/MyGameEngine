#pragma once
#include "CollisionManager.h"
#include "ObjectFBX.h"
#include "ObjectOBJ.h"
#include "SphereCollider.h"
#include "GameObject.h"


class Weapon : public ObjectFBX, public ObjectOBJ
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
	static Weapon* Create(ModelOBJ* obj = nullptr);

	/// <summary>
	/// FBXモデル生成
	/// </summary>
	/// <param name="fbx">モデルデータ</param>
	/// <param name="animationNumber">アニメーション番号</param>
	/// <returns>インスタンス</returns>
	static Weapon* Create(ModelFBX* fbx = nullptr, int animationNumber = 0);

	void SetParent(ObjectOBJ* obj) { this->ObjectOBJ::parent = obj; }
	void SetParent(ObjectFBX* fbx) { this->ObjectFBX::parent = fbx; }

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
	XMVECTOR move = { 0,0,0.1f,0 };
	// 落下ベクトル
	DirectX::XMVECTOR fallVel;
	// コライダー
	Sphere sphere{};
	SphereCollider* sphereColl = nullptr;
	// 攻撃用コライダー
	Sphere attackSphere{};
};
