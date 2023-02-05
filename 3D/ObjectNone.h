#pragma once
#include "CollisionInfo.h"

class BaseCollider;

// 何もオブジェクトがセットされていない時の
// 仮のオブジェクト(当たり判定用)
class ObjectNone
{
public:
	/// <summary>
	/// コライダーの設定
	/// </summary>
	void SetCollider(BaseCollider* collider);

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	virtual void OnCollision(const CollisionInfo& info) {}

protected: // メンバ変数
	// コライダー
	BaseCollider* collider = nullptr;
};