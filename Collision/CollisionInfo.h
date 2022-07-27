#pragma once

#include <DirectXMath.h>

class ObjectOBJ;
class ObjectFBX;
class BaseCollider;

/// <summary>
/// 衝突情報
/// </summary>
struct CollisionInfo
{
public:
	CollisionInfo(ObjectOBJ* obj, BaseCollider* collider, const DirectX::XMVECTOR& inter) {
		this->obj = obj;
		this->collider = collider;
		this->inter = inter;
	}

	CollisionInfo(ObjectFBX* fbx, BaseCollider* collider, const DirectX::XMVECTOR& inter) {
		this->fbx = fbx;
		this->collider = collider;
		this->inter = inter;
	}

	// 衝突相手のオブジェクト
	ObjectOBJ* obj = nullptr;
	ObjectFBX* fbx = nullptr;
	// 衝突相手のコライダー
	BaseCollider* collider = nullptr;
	// 衝突点
	DirectX::XMVECTOR inter;
};

