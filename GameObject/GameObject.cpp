#include "GameObject.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "Collision.h"
#include "SceneManager.h"

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
	sphereColl = new SphereCollider(sphere, true);

	SetPosition(pos);

	return true;
}

void GameObject::Update()
{
	Drag();
	Move();
	if(isDrag) CollisionManager::GetInstance()->CheckAllCollision(sphereColl, COLLISION_ATTR_OBJECT_SPHERE);

	SetPosition(pos);

	ObjectOBJ::Update();
	ObjectFBX::Update();
}

bool GameObject::Hit()
{
	return false;
}

void GameObject::Drag()
{
	if(SceneManager::GetScene() != EDIT) return;
	if (!used) return;

	Input* input = Input::GetInstance();

	if (input->PushMouse(MOUSE_LEFT) && isDrag)
	{

	}

	if (input->ReleaseMouse(MOUSE_LEFT) && isDrag)
	{
		//isDrag = false;
		//isDragStatic = false;

		//return;
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
		sphere.center = vec;
		sphereColl->SetOffset(sphere.center);
	}
}

void GameObject::Move()
{
	if (pos.x < -60) pos.x = -60;
	if (pos.x > 60) pos.x = 60;

	if (pos.z < -60) pos.z = -60;
	if (pos.z > 60) pos.z = 60;

	if (pos.y > 0) pos.y = 0;
}

void GameObject::SetPosition(const XMFLOAT3& position)
{
	this->pos = position;

	// コライダーの追加
	sphere.center = { pos.x, pos.y + 5.0f, pos.z,0 };
	sphere.radius = radius;

	if (ObjectOBJ::model)
	{
		ObjectOBJ::SetPosition(pos);

		if (isInGame)
		{
			meshColl->ConstructTriangle(ObjectOBJ::model);
			meshColl->SetAttribute(COLLISION_ATTR_OBJECT_MESH);

			if (!used) meshColl->SetAttribute(COLLISION_ATTR_OBJECT_NONE);

			ObjectOBJ::SetCollider(meshColl);
		}
		else
		{
			*sphereColl = SphereCollider(sphere, true);
			sphereColl->SetAttribute(COLLISION_ATTR_OBJECT_SPHERE);

			if (!used)
			{
				sphereColl->SetAttribute(COLLISION_ATTR_OBJECT_NONE);
			}

			ObjectOBJ::SetCollider(sphereColl);
		}
	}
	if (ObjectFBX::model)
	{
		ObjectFBX::SetPosition(pos);

		if (isInGame)
		{
			meshColl->ConstructTriangle(ObjectFBX::model);
			meshColl->SetAttribute(COLLISION_ATTR_OBJECT_MESH);

			if (!used) meshColl->SetAttribute(COLLISION_ATTR_OBJECT_NONE);

			ObjectFBX::SetCollider(meshColl);
		}
		else
		{
			*sphereColl = SphereCollider(sphere, true);
			sphereColl->SetAttribute(COLLISION_ATTR_OBJECT_SPHERE);

			if (!used) sphereColl->SetAttribute(COLLISION_ATTR_OBJECT_NONE);

			ObjectOBJ::SetCollider(sphereColl);
		}
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
	if (info.collider->GetAttribute() == COLLISION_ATTR_OBJECT_SPHERE && isDrag)
	{
		Rejection(info);
	}
}

void GameObject::Rejection(const CollisionInfo& info)
{
	pos.x -= info.reject.m128_f32[0];
	pos.z -= info.reject.m128_f32[2];
	//sphere.center -= info.reject;
}
