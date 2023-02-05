#pragma once
#include "CollisionInfo.h"

class BaseCollider;

// �����I�u�W�F�N�g���Z�b�g����Ă��Ȃ�����
// ���̃I�u�W�F�N�g(�����蔻��p)
class ObjectNone
{
public:
	/// <summary>
	/// �R���C�_�[�̐ݒ�
	/// </summary>
	void SetCollider(BaseCollider* collider);

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	virtual void OnCollision(const CollisionInfo& info) {}

protected: // �����o�ϐ�
	// �R���C�_�[
	BaseCollider* collider = nullptr;
};