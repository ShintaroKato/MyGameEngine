#include "Player.h"
#include "Input.h"

Player* Player::Create(ModelFBX* fbx, int animationNumber)
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
	if (fbx)
	{
		instance->SetModelFBX(fbx);
		instance->SetAnimationNumber(animationNumber);
		instance->AnimationReset();
	}

	return instance;
}

Player* Player::Create(ModelOBJ* obj)
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
	if (obj)
	{
		instance->SetModelOBJ(obj);
	}

	return instance;
}

bool Player::Initialize()
{
	ObjectFBX::Initialize();
	ObjectOBJ::Initialize();

	// �R���C�_�[�̒ǉ�
	float radius = 0.6f;
	collider = new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius);

	// ���a�������������畂�������W�����̒��S�ɂ���
	SetCollider(collider);
	collider->SetAttribute(COLLISION_ATTR_ALLIES);

	return true;
}

void Player::Update()
{
	Move();
	Attack();

	this->ObjectOBJ::Update();
	this->ObjectFBX::Update();
}

void Player::Move()
{
	Input* input = new Input();

	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) ||
		input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{
		if (input->PushKey(DIK_UP))
		{
		}
		else if (input->PushKey(DIK_DOWN))
		{
		}

		if (input->PushKey(DIK_RIGHT))
		{
		}
		else if (input->PushKey(DIK_LEFT))
		{
		}
	}

}

void Player::Attack()
{
}

bool Player::Hit()
{
	return false;
}

XMFLOAT3 Player::GetPosition()
{
	if (ObjectOBJ::model)
	{
		return ObjectOBJ::position;
	}
	if (ObjectFBX::model)
	{
		return ObjectFBX::position;
	}
}

void Player::SetPosition(XMFLOAT3 pos)
{
	if (ObjectOBJ::model)
	{
		ObjectOBJ::position = pos;
	}
	if (ObjectFBX::model)
	{
		ObjectFBX::position = pos;
	}
}

void Player::SetRotation(XMFLOAT3 rot)
{
	if (ObjectOBJ::model)
	{
		ObjectOBJ::rotation = rot;
	}
	if (ObjectFBX::model)
	{
		ObjectFBX::rotation = rot;
	}
}

void Player::SetScale(XMFLOAT3 scale)
{
	if (ObjectOBJ::model)
	{
		ObjectOBJ::scale = scale;
	}
	if (ObjectFBX::model)
	{
		ObjectFBX::scale = scale;
	}
}
