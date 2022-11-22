#include "Enemy.h"
#include "Input.h"
#include "SphereCollider.h"
#include "Collision.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

Enemy* Enemy::Create(ModelFBX* fbx, int animationNumber)
{
	// 3Dオブジェクトのインスタンスを生成
	Enemy* instance = new Enemy();
	if (instance == nullptr)
	{
		return nullptr;
	}

	// モデルのセット
	if (fbx)
	{
		instance->SetModelFBX(fbx);
		instance->SetAnimationNumber(animationNumber);
		instance->AnimationReset();
	}

	// 初期化
	if (!instance->Initialize())
	{
		delete instance;
		assert(0);
	}

	return instance;
}

Enemy* Enemy::Create(ModelOBJ* obj)
{
	// 3Dオブジェクトのインスタンスを生成
	Enemy* instance = new Enemy();
	if (instance == nullptr)
	{
		return nullptr;
	}

	// モデルのセット
	if (obj)
	{
		instance->SetModelOBJ(obj);
	}

	// 初期化
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

	// コライダーの追加
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
	if (abs(pos.x - targetPos.x) <= 2 && abs(pos.z - targetPos.z) <= 2 &&
		aliveFlag == true)
	{
		aliveFlag = false;
	}
}

void Enemy::Move()
{
	// 標的の方向を向く
	rotation.y = XMConvertToDegrees(
		atan2f(targetPos.x - pos.x, targetPos.z - pos.z));

	move = { 0,0,0.1f,0 };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotation.y));
	move = XMVector3TransformNormal(move, matRot);

	float nearLimit = 5.0f;

	//ある程度の距離を保ち続ける処理
	if (abs(pos.x - targetPos.x) < nearLimit && abs(pos.z - targetPos.z) < nearLimit);
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
	ObjectOBJ::SetPower(attackPower);
	CollisionManager::GetInstance()->CheckAllCollision(sphereColl, COLLISION_ATTR_OBJECT_MESH);
}

void Enemy::Hit(float attackPower)
{
	HP -= attackPower;

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
	if (info.collider->GetAttribute() == COLLISION_ATTR_OBJECT_MESH)
	{
		if (info.obj) targetPos = info.obj->GetPosition();
		else if (info.fbx) targetPos = info.obj->GetPosition();
	}
	else if (info.collider->GetAttribute() == COLLISION_ATTR_OBJECT_MESH)
	{
		SetTargetPos(target);
	}
	if (info.collider->GetAttribute() == COLLISION_ATTR_WEAPONS + COLLISION_ATTR_ALLIES)
	{
		if(info.obj) Hit(info.obj->attackPower);
		else if(info.fbx) Hit(info.fbx->attackPower);
	}
}

void Enemy::SetPosition(XMFLOAT3 pos)
{
	// コライダーの追加
	sphere.center = { pos.x, pos.y + radius, pos.z,0 };
	sphere.radius = radius;

	sphereColl->SetSphere(sphere);

	if (ObjectOBJ::model)
	{
		ObjectOBJ::SetPosition(pos);
		ObjectOBJ::collider = sphereColl;
	}
	if (ObjectFBX::model)
	{
		ObjectFBX::SetPosition(pos);
		ObjectFBX::collider = sphereColl;
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
