#include "Weapon.h"
#include "Input.h"
#include "SphereCollider.h"
#include "Collision.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

Weapon* Weapon::Create(ModelFBX* fbx, int animationNumber)
{
	// 3Dオブジェクトのインスタンスを生成
	Weapon* instance = new Weapon();
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

Weapon* Weapon::Create(ModelOBJ* obj)
{
	// 3Dオブジェクトのインスタンスを生成
	Weapon* instance = new Weapon();
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

bool Weapon::Initialize()
{
	ObjectFBX::Initialize();
	ObjectOBJ::Initialize();

	// コライダーの追加
	sphere.center = { pos.x, pos.y + radius, pos.z,0 };
	sphere.radius = radius;
	sphereColl = new SphereCollider(sphere);

	if (ObjectOBJ::model) ObjectOBJ::SetCollider(sphereColl);
	if (ObjectFBX::model) ObjectFBX::SetCollider(sphereColl);

	return true;
}

void Weapon::SetParent(ObjectOBJ* obj)
{
	this->obj = obj;
	sphereColl->SetAttribute(obj->GetCollider()->GetAttribute() + 0b1);
}

void Weapon::SetParent(ObjectFBX* fbx)
{
	this->fbx = fbx;
	sphereColl->SetAttribute(fbx->GetCollider()->GetAttribute() + 0b1);
}

void Weapon::Update()
{
	if (ObjectOBJ::model)
	{
		pos = ObjectOBJ::position;
		rotation = ObjectOBJ::rotation;
	}
	if (ObjectFBX::model)
	{
		pos = ObjectFBX::position;
		rotation = ObjectFBX::rotation;
	}

	SetPosition(pos);

	if (ObjectOBJ::model)
	{
		ObjectOBJ::collider->Update();
	}
	if (ObjectFBX::model)
	{
		ObjectFBX::collider->Update();
	}
	ObjectOBJ::Update();
	ObjectFBX::Update();
}

void Weapon::OnCollision(const CollisionInfo& info)
{
}

void Weapon::SetPosition(XMFLOAT3 pos)
{
	// コライダーの追加
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

void Weapon::SetRotation(XMFLOAT3 rot)
{
	if (ObjectOBJ::model)
	{
		ObjectOBJ::SetRotation(rot);
	}
	if (ObjectFBX::model)
	{
		ObjectFBX::SetRotation(rot);
	}
}

void Weapon::SetScale(XMFLOAT3 scale)
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