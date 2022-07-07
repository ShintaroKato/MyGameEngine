#include "Player.h"

Player* Player::Create(ModelFBX* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Player* instance = new Player();
	if (instance == nullptr)
	{
		return nullptr;
	}

	// ������
	if (!instance->Initialize())
	{
		delete instance;
		assert(0);
	}

	// ���f���̃Z�b�g
	if (model)
	{
		instance->SetModel(model);
	}

	return instance;
}

bool Player::Initialize()
{
	if (!ObjectFBX::Initialize())
	{
		return false;
	}

	// �R���C�_�[�̒ǉ�
	/*float radius = 0.6f;
	collider = new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius);*/

	// ���a�������������畂�������W�����̒��S�ɂ���
	/*SetCollider(collider);
	collider->SetAttribute(COLLISION_ATTR_ALLIES);*/

	return true;
}

void Player::Update()
{
	Move();
	Attack();
}

void Player::Move()
{
	
}

void Player::Attack()
{
}

bool Player::Hit()
{
	return false;
}
