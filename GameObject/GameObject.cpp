#include "GameObject.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "Collision.h"

bool GameObject::isDragStatic = false;

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
	sphere.center = { pos.x, pos.y + radius, pos.z,0 };
	sphere.radius = radius;

	meshColl = new MeshCollider();
	sphereColl = new SphereCollider();

	SetPosition(pos);
	meshColl->SetAttribute(COLLISION_ATTR_OBJECT);
	sphereColl->SetAttribute(COLLISION_ATTR_OBJECT);
	sphereColl->SetSphere(sphere);

	return true;
}

void GameObject::Update()
{
	Drag();
	Move();

	ObjectOBJ::Update();
	ObjectFBX::Update();

	 if(ObjectOBJ::model) ObjectOBJ::collider->Update();
	 if(ObjectFBX::model) ObjectFBX::collider->Update();
}

bool GameObject::Hit()
{
	return false;
}

void GameObject::Drag()
{
	if(isInGame) return;

	Input* input = Input::GetInstance();

	if (input->ReleaseMouse(MOUSE_LEFT) && isDrag)
	{
		CollisionManager::GetInstance()->CheckAllCollision(sphereColl, COLLISION_ATTR_OBJECT);

		if (pos.x < -60) pos.x = -60;
		if (pos.x > 60) pos.x = 60;

		if (pos.z < -60) pos.z = -60;
		if (pos.z > 60) pos.z = 60;

		if (pos.y > 0) pos.y = 0;

		SetPosition(pos);

		isDrag = false;
		isDragStatic = false;

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
			if(!isDragStatic) isDrag = true;
			isDragStatic = true;
		}
	}
	// 掴まれている時の処理
	if (isDrag)
	{
		pos = { vec.m128_f32[0], vec.m128_f32[1], vec.m128_f32[2] };
		SetPosition(pos);
	}
}

void GameObject::Move()
{
	if(!isDrag) return;

	SetPosition(pos);
}

void GameObject::SetPosition(const XMFLOAT3& pos)
{
	// コライダーの追加
	sphere.center = { pos.x, pos.y + radius, pos.z,0 };
	sphere.radius = radius;

	sphereColl->SetSphere(sphere);

	if (ObjectOBJ::model)
	{
		meshColl->ConstructTriangle(ObjectOBJ::model);
		ObjectOBJ::SetPosition(pos);
		ObjectOBJ::SetCollider(sphereColl);
	}
	if (ObjectFBX::model)
	{
		meshColl->ConstructTriangle(ObjectFBX::model);
		ObjectFBX::SetPosition(pos);
		if (!isInGame) ObjectFBX::SetCollider(sphereColl);
	}
}

XMFLOAT3 GameObject::GetPosition()
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

void GameObject::OnCollision(const CollisionInfo& info)
{
	if (info.collider->GetAttribute() == COLLISION_ATTR_OBJECT)
	{
		Rejection(info);
	}
}

void GameObject::Rejection(const CollisionInfo& info)
{
	XMVECTOR vec = {
		info.inter.m128_f32[0] / 2,
		info.inter.m128_f32[1] / 2,
		info.inter.m128_f32[2] / 2
	};

	pos.x -= vec.m128_f32[0];
	pos.z -= vec.m128_f32[2];

	SetPosition(pos);
}
