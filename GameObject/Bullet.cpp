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
	sphere.center = { position.x, position.y, position.z,0 };
	sphere.radius = radius;
	sphereColl = new SphereCollider(sphere);
	sphereColl->SetAttribute(collAttribute);

	CollisionManager::GetInstance()->AddCollider(sphereColl);

	return true;
}

void Bullet::Update()
{
	SetPosition(position);
}

void Bullet::OnCollision(const CollisionInfo& info)
{
	if (info.collider->GetAttribute() == COLLISION_ATTR_ENEMIES ||
		info.collider->GetAttribute() == COLLISION_ATTR_LANDSHAPE ||
		info.collider->GetAttribute() == COLLISION_ATTR_OBJECT_MESH)
	{
		frame += end_frame;
	}
}

void Bullet::SetPosition(XMFLOAT3 pos)
{
	// コライダーの追加
	sphere.center = { pos.x, pos.y + radius, pos.z,0 };
	sphere.radius = radius;

	sphereColl->SetSphere(sphere);
}

void Bullet::SetRotation(XMFLOAT3 rot)
{

}

void Bullet::SetScale(XMFLOAT3 scale)
{

}