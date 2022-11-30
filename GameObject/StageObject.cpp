#include "StageObject.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "Collision.h"
#include "SceneManager.h"
#include "PlaneCursor.h"

bool StageObject::isDragStatic = false;

StageObject* StageObject::Create(ModelFBX* fbx)
{
	// 3Dオブジェクトのインスタンスを生成
	StageObject* instance = new StageObject();
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

StageObject* StageObject::Create(ModelOBJ* obj)
{
	// 3Dオブジェクトのインスタンスを生成
	StageObject* instance = new StageObject();
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

bool StageObject::Initialize()
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

void StageObject::Update()
{
	if (HP <= 0) aliveFlag = false;

	if (!isInGame)
	{
		Drag();
		Move();
	}

	SetPosition(pos);

	ObjectOBJ::Update();
	ObjectFBX::Update();
}

void StageObject::Draw()
{
	if (!aliveFlag) return;

	ObjectOBJ::Draw();
}

void StageObject::Hit(float attackPower)
{
	HP -= attackPower;

	if (HP <= 0)
	{
		aliveFlag = false;
	}
}

void StageObject::Drag()
{
	if(SceneManager::GetScene() != EDIT) return;
	if (used == UNUSED) return;

	Input* input = Input::GetInstance();

	if (!CollisionManager::GetInstance()->CheckAllCollision(sphereColl, COLLISION_ATTR_OBJECT_SPHERE) &&
		input->TriggerMouse(MOUSE_LEFT) && isDrag)
	{
		isDrag = false;
		isDragStatic = false;

		if (used == WAITING) used = USED;

		PlaneCursor::SetIsDrag(isDragStatic);
	}
	else if (CollisionManager::GetInstance()->CheckAllCollision(sphereColl, COLLISION_ATTR_PLANE_CURSOR) &&
		input->TriggerMouse(MOUSE_LEFT) && !isDrag ||
		used == WAITING)
	{
		// 掴んでいる状態にする
		if(!isDragStatic) isDrag = true;
		isDragStatic = true;
		PlaneCursor::SetIsDrag(isDragStatic);
	}

	// 掴まれている時の処理
	if (isDrag)
	{
		PlaneCursor::SetRadius(radius);
		pos = PlaneCursor::GetPosition();
		pos.y += 10.0f;

		sphere.center = { pos.x, pos.y, pos.z };
		sphereColl->SetOffset(sphere.center);
	}

	if (PlaneCursor::GetIsDrag())
	{
		if (CollisionManager::GetInstance()->CheckAllCollision(sphereColl, COLLISION_ATTR_OBJECT_SPHERE))
		{
			PlaneCursor::SetColor({ 0.0f,1.0f,0.0f,0.0f });
		}
		else
		{
			PlaneCursor::SetColor({ 0.0f,0.0f,1.0f,0.0f });
		}
	}
	else
	{
		PlaneCursor::SetColor({ 0.0f,1.0f,1.0f,1.0f });

	}
}

void StageObject::Move()
{
	if (pos.x < -60) pos.x = -60;
	if (pos.x > 60) pos.x = 60;

	if (pos.z < -60) pos.z = -60;
	if (pos.z > 60) pos.z = 60;

	if (!isDrag) pos.y = 0;
}

void StageObject::SetPosition(const XMFLOAT3& position)
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

			if (!used || !aliveFlag) meshColl->SetAttribute(COLLISION_ATTR_OBJECT_NONE);

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

			if (!used || !aliveFlag) meshColl->SetAttribute(COLLISION_ATTR_OBJECT_NONE);

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

XMFLOAT3 StageObject::GetPosition()
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

void StageObject::OnCollision(const CollisionInfo& info)
{
	if (info.collider->GetAttribute() == COLLISION_ATTR_OBJECT_SPHERE && isDrag)
	{
		//Rejection(info);
	}
	if (info.collider->GetAttribute() == COLLISION_ATTR_ENEMIES && tag != "default")
	{
		if (info.obj) Hit(info.obj->attackPower);
		else if (info.fbx) Hit(info.fbx->attackPower);
	}
}

void StageObject::Rejection(const CollisionInfo& info)
{
	pos.x -= info.reject.m128_f32[0];
	pos.z -= info.reject.m128_f32[2];

	hit = true;
}