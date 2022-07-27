#pragma once

#include <DirectXMath.h>

class ObjectOBJ;
class ObjectFBX;
class BaseCollider;

/// <summary>
/// �Փˏ��
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

	// �Փˑ���̃I�u�W�F�N�g
	ObjectOBJ* obj = nullptr;
	ObjectFBX* fbx = nullptr;
	// �Փˑ���̃R���C�_�[
	BaseCollider* collider = nullptr;
	// �Փ˓_
	DirectX::XMVECTOR inter;
};

