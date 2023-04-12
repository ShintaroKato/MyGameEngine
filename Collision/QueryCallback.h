#pragma once

#include <DirectXMath.h>

class ObjectOBJ;
class ObjectFBX;
class BaseCollider;

/// <summary>
/// �N�G���ɂ����𓾂邽�߂̍\����
/// </summary>
struct QueryHit
{
	// �Փˑ���̃I�u�W�F�N�g
	ObjectOBJ* obj = nullptr;
	ObjectFBX* fbx = nullptr;
	// �Փˑ���̃R���C�_�[
	BaseCollider* collider = nullptr;
	// �Փ˓_
	DirectX::XMVECTOR inter;
	// �r�˃x�N�g��
	DirectX::XMVECTOR reject;
};

/// <summary>
/// �N�G���Ō��������o�����Ƃ��̓�����K�肷��N���X
/// </summary>
class QueryCallback
{
public:
	QueryCallback() = default;
	virtual ~QueryCallback() = default;

	/// <summary>
	/// �������R�[���o�b�N
	/// </summary>
	/// <param name="info">�������</param>
	/// <returns>�N�G���𑱂���Ȃ�true, �ł��؂�Ȃ�false</returns>
	virtual bool OnQueryHit(const QueryHit& info) = 0;
};