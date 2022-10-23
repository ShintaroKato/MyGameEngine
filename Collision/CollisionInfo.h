#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class ObjectOBJ;
class ObjectFBX;
class BaseCollider;

/// <summary>
/// �Փˏ��
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

	// �Փˑ���̃I�u�W�F�N�g
	ObjectOBJ* obj = nullptr;
	ObjectFBX* fbx = nullptr;
	// �Փˑ���̃R���C�_�[
	BaseCollider* collider = nullptr;
	// �Փ˓_
	XMVECTOR inter;
	// �����o���p
	XMVECTOR reject;
};

