#include "Bullet.h"
#include "Input.h"
#include "SphereCollider.h"
#include "Collision.h"
#include "CollisionManager.h"

Bullet* Bullet::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	Bullet* instance = new Bullet();
	if (instance == nullptr)
	{
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize())
	{
		delete instance;
		assert(0);
	}

	return instance;
}

bool Bullet::Initialize()
{
	// コライダーの追加
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
	// 何かに当たったらその場で消滅
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
	// コライダーの追加
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