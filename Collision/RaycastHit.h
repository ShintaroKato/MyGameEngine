#pragma once

#include "BaseCollider.h"
#include <DirectXMath.h>

class ObjectOBJ;
class ObjectFBX;

/// <summary>
/// ���C�L���X�g�ɂ����𓾂�ׂ̍\����
/// </summary>
struct RaycastHit
{
	// �Փˑ���̃I�u�W�F�N�g
	ObjectOBJ* obj = nullptr;
	ObjectFBX* fbx = nullptr;
	// �Փˑ���̃R���C�_�[
	BaseCollider* collider = nullptr;
	// �Փ˓_
	DirectX::XMVECTOR inter;
	// �Փ˓_�܂ł̋���
	float distance = 0.0f;
};

