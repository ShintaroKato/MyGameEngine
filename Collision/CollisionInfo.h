#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class ObjectOBJ;
class ObjectFBX;
class BaseCollider;

/// <summary>
/// 衝突情報
/// </summary>
struct CollisionInfo
{
public:
	CollisionInfo(ObjectOBJ* obj, BaseCollider* collider, const XMVECTOR& inter, const XMVECTOR& reject) {
		this->obj = obj;
		this->collider = collider;
		this->inter = inter;
		this->reject = reject;
	}

	CollisionInfo(ObjectFBX* fbx, BaseCollider* collider, const XMVECTOR& inter, const XMVECTOR& reject) {
		this->fbx = fbx;
		this->collider = collider;
		this->inter = inter;
		this->reject = reject;
	}

	// 衝突相手のオブジェクト
	ObjectOBJ* obj = nullptr;
	ObjectFBX* fbx = nullptr;
	// 衝突相手のコライダー
	BaseCollider* collider = nullptr;
	// 衝突点
	XMVECTOR inter;
	// 押し出し用
	XMVECTOR reject;
};

