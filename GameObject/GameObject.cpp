#include "GameObject.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "Collision.h"

GameObject* GameObject::Create(ModelFBX* fbx)
{
	// 3Dオブジェクトのインスタンスを生成
	GameObject* instance = new GameObject();
	if (instance == nullptr)
	{
		return nullptr;
	}

	// モデルのセット
	if (fbx)
	{
		instance->SetModelFBX(fbx);
	}

	// 初期化
	if (!instance->Initialize())
	{
		delete instance;
		assert(0);
	}

	return instance;
}

GameObject* GameObject::Create(ModelOBJ* obj)
{
	// 3Dオブジェクトのインスタンスを生成
	GameObject* instance = new GameObject();
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

bool GameObject::Initialize()
{
	ObjectFBX::Initialize();
	ObjectOBJ::Initialize();

	// コライダーの追加
	float radius = 5.0f;
	sphere.center = { pos.x, pos.y + radius, pos.z,0 };
	sphere.radius = radius;
	collider = new SphereCollider(sphere.center, sphere.radius);

	if (ObjectOBJ::model) ObjectOBJ::SetCollider(collider);
	if (ObjectFBX::model) ObjectFBX::SetCollider(collider);
	collider->SetAttribute(COLLISION_ATTR_OBJECT);

	return true;
}

void GameObject::Update()
{
	Drag();
	Move();

	ObjectOBJ::Update();
	ObjectOBJ::SetCollider(collider);

	ObjectFBX::Update();
	ObjectFBX::SetCollider(collider);
}

void GameObject::Move()
{
	// 掴まれていなければ関数を抜ける
	if (!isDrag) return;

	if (ObjectOBJ::model != nullptr)
	{
		ObjectOBJ::position = pos;
	}
	else if (ObjectFBX::model != nullptr)
	{
		ObjectFBX::position = pos;
	}
}

bool GameObject::Hit()
{
	return false;
}

void GameObject::Drag()
{
	if(fix) return;

	Input* input = Input::GetInstance();

	if (!input->PushMouse(MOUSE_LEFT))
	{
		isDrag = false;
		return;
	}

	XMVECTOR vec = input->CursorPoint3D(Camera::GetViewMatrix(), Camera::GetProjectionMatrix());
	Ray ray;
	ray.start = input->CalcScreenToWorld(input->GetMousePos2(), 0.0f, Camera::GetViewMatrix(), Camera::GetProjectionMatrix());
	ray.dir = input->CalcScreenToWorld(input->GetMousePos2(), 1.0f, Camera::GetViewMatrix(), Camera::GetProjectionMatrix());
	ray.dir = XMVector3Normalize(ray.dir - ray.start);
	float distance = 0;
	XMVECTOR inter{};

	if (input->TriggerMouse(MOUSE_LEFT))
	{
		if (Collision::ChackRay2Sphere(ray, sphere, &distance, &inter))
		{
			// 掴んでいる状態にする
			isDrag = true;
		}
	}
	// 掴まれている時の処理
	if (isDrag)
	{
		pos = { vec.m128_f32[0], vec.m128_f32[1], vec.m128_f32[2] };
		SetPosition(pos);
	}
}

void GameObject::SetPosition(const XMFLOAT3& pos)
{
	this->pos = pos;

	// コライダーの追加
	float radius = 5.0f;
	sphere.center = { pos.x, pos.y + radius, pos.z,0 };
	sphere.radius = radius;
	collider = new SphereCollider(sphere.center, sphere.radius);

	if (ObjectOBJ::model)
	{
		ObjectOBJ::SetPosition(pos);
		ObjectOBJ::SetCollider(collider);
	}
	if (ObjectFBX::model)
	{
		ObjectFBX::SetPosition(pos);
		ObjectFBX::SetCollider(collider);
	}
}

void GameObject::OnCollision(const CollisionInfo& info)
{
}
