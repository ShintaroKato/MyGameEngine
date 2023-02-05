#include "ObjectNone.h"
#include "BaseCollider.h"
#include "CollisionManager.h"

void ObjectNone::SetCollider(BaseCollider* collider)
{
	collider->SetObjectNone(this);
	this->collider = collider;
	// �R���W�����}�l�[�W���ɒǉ�
	CollisionManager::GetInstance()->AddCollider(this->collider);
	// �R���C�_�[���X�V���Ă���
	this->collider->Update();
}
