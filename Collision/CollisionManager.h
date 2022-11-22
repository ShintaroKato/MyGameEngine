#pragma once
#include "CollisionPrimitive.h"
#include "RaycastHit.h"

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
	bool CheckAllCollision(BaseCollider* col);
	bool CheckAllCollision(BaseCollider* col, unsigned short attr);

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

private:
	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	~CollisionManager() = default;
	CollisionManager& operator=(const CollisionManager&) = delete;
	//コライダーのリスト
	std::forward_list<BaseCollider*>colliders;
};

