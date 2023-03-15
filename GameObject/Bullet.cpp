#include "Bullet.h"
#include "Input.h"
#include "SphereCollider.h"
#include "Collision.h"
#include "CollisionManager.h"

Bullet* Bullet::Create()
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Bullet* instance = new Bullet();
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

	return instance;
}

bool Bullet::Initialize()
{
	// �R���C�_�[�̒ǉ�
	sphere.center = XMLoadFloat3(&position);
	sphere.radius = radius;
	sphereColl = new SphereCollider(sphere);
	sphereColl->SetAttribute(collAttribute);

	SetCollider(sphereColl);

	if (collAttribute == COLLISION_ATTR_BULLET + COLLISION_ATTR_ALLIES)
	{
		return true;
	}

	return true;
}

void Bullet::Update()
{
	SetPosition(position);
}

void Bullet::OnCollision(const CollisionInfo& info)
{
	// �����ɓ��������炻�̏�ŏ���
	if (!through &&(
			info.collider->GetAttribute() == COLLISION_ATTR_ENEMIES ||
			info.collider->GetAttribute() == COLLISION_ATTR_OBJECT_MESH 
		))
	{
		frame += end_frame;
	}
}

void Bullet::SetPosition(XMFLOAT3 pos)
{
	// �R���C�_�[�̒ǉ�
	sphere.center = XMLoadFloat3(&pos);
	sphere.center.m128_f32[1] += radius;
	sphere.radius = radius;

	sphereColl->SetSphere(sphere);

	SetCollider(sphereColl);
}

void Bullet::SetRotation(XMFLOAT3 rot)
{

}

void Bullet::SetScale(XMFLOAT3 scale)
{

}