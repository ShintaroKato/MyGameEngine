#include "Enemy.h"
#include "Input.h"
#include "SphereCollider.h"
#include "Collision.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

Enemy* Enemy::Create(ModelFBX* fbx, int animationNumber)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Enemy* instance = new Enemy();
	if (instance == nullptr)
	{
		return nullptr;
	}

	// ���f���̃Z�b�g
	if (fbx)
	{
		instance->SetModelFBX(fbx);
		instance->SetAnimationNumber(animationNumber);
		instance->AnimationReset();
	}

	// ������
	if (!instance->Initialize())
	{
		delete instance;
		assert(0);
	}

	return instance;
}

Enemy* Enemy::Create(ModelOBJ* obj)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Enemy* instance = new Enemy();
	if (instance == nullptr)
	{
		return nullptr;
	}

	// ���f���̃Z�b�g
	if (obj)
	{
		instance->SetModelOBJ(obj);
	}

	// ������
	if (!instance->Initialize())
	{
		delete instance;
		assert(0);
	}

	return instance;
}

bool Enemy::Initialize()
{
	ObjectFBX::Initialize();
	ObjectOBJ::Initialize();

	// �R���C�_�[�̒ǉ�
	sphere.center = { pos.x, pos.y + radius, pos.z,0 };
	sphere.radius = radius;
	sphereColl = new SphereCollider(sphere);

	if (ObjectOBJ::model) ObjectOBJ::SetCollider(sphereColl);
	if (ObjectFBX::model) ObjectFBX::SetCollider(sphereColl);
	sphereColl->SetAttribute(COLLISION_ATTR_ENEMIES);

	return true;
}

void Enemy::Update()
{
	if (!isInGame) return;

	Spawn();

	if (!aliveFlag) return;

	if (ObjectOBJ::model) pos = ObjectOBJ::position;
	if (ObjectFBX::model) pos = ObjectFBX::position;

	Move();
	Jump();
	Attack();

	SetPosition(pos);

	if (ObjectOBJ::model)
	{
		ObjectOBJ::rotation = rotation;
		ObjectOBJ::collider->Update();
	}
	if (ObjectFBX::model)
	{
		ObjectFBX::rotation = rotation;
		ObjectFBX::collider->Update();
	}
	ObjectOBJ::Update();
	ObjectFBX::Update();
}

void Enemy::Spawn()
{
	if (aliveFlag == false)
	{
		HP = 20.0f;

		float rad = XMConvertToRadians(rand() % 360);
		float distance = 50;

		pos.x = distance * sin(rad);
		pos.z = distance * cos(rad);

		SetPosition(pos);

		aliveFlag = true;
	}
	if (abs(pos.x - target.x) <= 2 && abs(pos.z - target.z) <= 2 &&
		aliveFlag == true)
	{
		aliveFlag = false;
	}
}

void Enemy::Move()
{
	// �W�I�̕���������
	rotation.y = XMConvertToDegrees(
		atan2f(target.x - pos.x, target.z - pos.z));

	move = { 0,0,0.1f,0 };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotation.y));
	move = XMVector3TransformNormal(move, matRot);

	float nearLimit = 5.5f;

	//������x�̋�����ۂ������鏈��
	if (abs(pos.x - target.x) < nearLimit && abs(pos.z - target.z) < nearLimit);
	else
	{
		pos.x += move.m128_f32[0];
		pos.y += move.m128_f32[1];
		pos.z += move.m128_f32[2];
	}
}

void Enemy::Jump()
{

}

void Enemy::Attack()
{
}

void Enemy::Hit()
{
	HP -= 100.0f;

	if (HP <= 0)
	{
		aliveFlag = false;
	}
}

XMFLOAT3 Enemy::GetPosition()
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

void Enemy::OnCollision(const CollisionInfo& info)
{
	if (info.collider->GetAttribute() == COLLISION_ATTR_WEAPONS + COLLISION_ATTR_ALLIES)
	{
		Hit();
	}
}

void Enemy::SetPosition(XMFLOAT3 pos)
{
	// �R���C�_�[�̒ǉ�
	sphere.center = { pos.x, pos.y + radius, pos.z,0 };
	sphere.radius = radius;

	sphereColl->SetSphere(sphere);

	if (ObjectOBJ::model)
	{
		ObjectOBJ::SetPosition(pos);
		ObjectOBJ::SetCollider(sphereColl);
	}
	if (ObjectFBX::model)
	{
		ObjectFBX::SetPosition(pos);
		ObjectFBX::SetCollider(sphereColl);
	}
}

void Enemy::SetRotation(XMFLOAT3 rot)
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

void Enemy::SetScale(XMFLOAT3 scale)
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
