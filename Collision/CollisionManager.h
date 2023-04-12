#pragma once
#include "CollisionPrimitive.h"
#include "RaycastHit.h"
#include "QueryCallback.h"

#include <d3d12.h>
#include <forward_list>
#include <algorithm>

class BaseCollider;

class CollisionManager
{
public: //静的メンバ関数
	static CollisionManager* GetInstance();

public: //メンバ関数

	/// <summary>
	/// コライダーの追加
	/// </summary>
	/// <param name="collider">コライダー</param>
	inline void AddCollider(BaseCollider* collider)
	{
		// リスト内の要素を探索
		auto result = std::find(colliders.begin(), colliders.end(), collider);
		// 指定した要素が見つからなかったらリストに要素を追加
		if(result == colliders.end()) colliders.push_front(collider);
	}

	/// <summary>
	/// コライダーの削除
	/// </summary>
	/// <param name="collider">コライダー</param>
	inline void RemoveCollider(BaseCollider* collider)
	{
		colliders.remove(collider);
	}

	/// <summary>
	/// コライダーを全て削除
	/// </summary>
	inline void DeleteCollider()
	{
		colliders.clear();
	}

	/// <summary>
	/// 全ての衝突チェック
	/// </summary>
	bool CheckAllCollision();

	/// <summary>
	/// 特定のコライダーと、それ以外の全てのコライダーとの衝突をチェック
	/// </summary>
	/// <param name="col">コライダー</param>
	bool CheckAllCollision(BaseCollider* col);

	/// <summary>
	/// 特定のコライダーと、指定した属性を持つ全てのコライダーとの衝突をチェック
	/// </summary>
	/// <param name="col">コライダー</param>
	/// <param name="attr">当たり判定属性</param>
	bool CheckAllCollision(BaseCollider* col, unsigned short attr);

	/// <summary>
	///	特定のコライダーと、指定したコライダーを除く全てのコライダーとの衝突をチェック
	/// </summary>
	/// <param name="col">コライダー</param>
	/// <param name="ex_col">除外するコライダー</param>
	bool CheckAllCollision(BaseCollider* col, BaseCollider* ex_col);

	/// <summary>
	///
	/// </summary>
	/// <returns>判定</returns>
	bool CheckCollision(BaseCollider* colA, BaseCollider* colB);

	/// <summary>
	/// セットされているオブジェクトを判別
	/// </summary>
	void CheckSetObject(BaseCollider* colA, BaseCollider* colB, XMVECTOR inter, XMVECTOR reject = {});

	/// <summary>
	/// レイキャスト
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="hitInfo">衝突情報</param>
	/// <param name="maxDistance">最大距離</param>
	/// <returns>判定</returns>
	bool Raycast(const Ray& ray, RaycastHit* hitInfo = nullptr,
		float maxDistance = D3D12_FLOAT32_MAX);

	/// <summary>
	/// レイキャスト
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="attribute">対象の衝突属性</param>
	/// <param name="hitInfo">衝突情報</param>
	/// <param name="maxDistance">最大距離</param>
	/// <returns>判定</returns>
	bool Raycast(const Ray& ray, unsigned short attribute, RaycastHit* hitInfo = nullptr,
		float maxDistance = D3D12_FLOAT32_MAX);

	/// <summary>
	/// レイキャスト
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="ex_attribute">除外する衝突属性</param>
	/// <param name="ex_col">除外するコライダー</param>
	/// <param name="hitInfo">衝突情報</param>
	/// <param name="maxDistance">最大距離</param>
	/// <returns>判定</returns>
	bool Raycast(const Ray& ray, unsigned short ex_attribute, BaseCollider* ex_col, RaycastHit* hitInfo = nullptr,
		float maxDistance = D3D12_FLOAT32_MAX);

	/// <summary>
	/// 球による衝突全検索
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="callback">衝突時コールバック</param>
	/// <param name="attribute">対象の衝突属性</param>
	void QuerySphere(const Sphere& sphere, QueryCallback* callback, unsigned short attribute = (unsigned short)0xffff'ffff);

private:
	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	~CollisionManager() = default;
	CollisionManager& operator=(const CollisionManager&) = delete;
	//コライダーのリスト
	std::forward_list<BaseCollider*>colliders;
};

