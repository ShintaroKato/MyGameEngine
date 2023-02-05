#include "StageObject.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "Collision.h"
#include "SceneManager.h"
#include "PlaneCursor.h"
#include "ParticleEmitter.h"
#include "BulletManager.h"

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
	sphere.center = XMLoadFloat3(&pos);
	sphere.center.m128_f32[1] += radius;
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
		ResetStatus();
		Drag();
		Move();
		Rotation();
		ChangeDefault();
	}
	if (tag == OFFENCE_OBJECT)
	{
		Attack();
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
		PlaneCursor::SetSquareSideLength(
			radius + sphereColl->GetSquareSideLength().x,
			radius + sphereColl->GetSquareSideLength().y);

		pos = PlaneCursor::GetPosition();
		pos.y += 10.0f;

		sphere.center = XMLoadFloat3(&pos);
		sphereColl->SetOffset(sphere.center);
	}

	if (PlaneCursor::GetIsDrag() && isDrag)
	{
		PlaneCursor::SetColor({ 0.0f,1.0f,0.0f,0.0f }); // 緑

		if (CollisionManager::GetInstance()->CheckAllCollision(sphereColl, COLLISION_ATTR_OBJECT_SPHERE))
		{
			PlaneCursor::SetColor({ 1.0f,0.0f,0.0f,0.0f }); // 赤
		}
	}
	else if(!PlaneCursor::GetIsDrag())
	{
		PlaneCursor::SetColor({ 1.0f,1.0f,1.0f,1.0f }); // 白
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

void StageObject::Rotation()
{
	Input* input = Input::GetInstance();

	if (!isDrag) return;

	if (input->TriggerKey(DIK_A))		rot.y -= 90.0f;
	else if (input->TriggerKey(DIK_D))	rot.y += 90.0f;

	if (abs(rot.y) >= 360) rot.y = 0;

	if (ObjectOBJ::model) ObjectOBJ::SetRotation(rot);
	if (ObjectFBX::model) ObjectFBX::SetRotation(rot);
}

void StageObject::ChangeDefault()
{
	if (!isDrag || tag == CASTLE_OBJECT) return;

	Input* input = Input::GetInstance();

	if (input->TriggerKey(DIK_SPACE))
	{
		isDrag = false;
		isDragStatic = false;
		PlaneCursor::SetIsDrag(isDragStatic);

		tag = STAGE_OBJECT_DEFAULT;
		used = UNUSED;
	}
}

void StageObject::ObjectType()
{
	switch (tag)
	{
	case CASTLE_OBJECT:
		radius = 5.0f;
		HPMax = 500;
		HP = HPMax;
		break;

	case RED_OBJECT:
		radius = 5.0f;
		HPMax = 1200;
		HP = HPMax;
		break;

	case GREEN_OBJECT:
		radius = 3.0f;
		HPMax = 100;
		HP = HPMax;
		break;

	case BLUE_OBJECT:
		radius = 3.5f;
		HPMax = 800;
		HP = HPMax;
		break;

	case OFFENCE_OBJECT:
		sensor.Initialize(pos, 10);
		radius = 3.0f;
		HPMax = 100;
		HP = HPMax;
		break;

	case STAGE_OBJECT_DEFAULT:
	default:
		radius = 0.0f;
		HPMax = 0;
		HP = HPMax;
		break;
	}
}

void StageObject::Attack()
{
	//if()
	if (attackCount > 0)
	{
		attackCount--;
	}
	else
	{
		//rot.x = cos sensor.GetTargetPos();

		BulletManager::GetInstance()->Fire(pos, rot, COLLISION_ATTR_ALLIES, 1.0f, power);
		attackCount = attackCountMax;
	}
}

void StageObject::SetPosition(const XMFLOAT3& position)
{
	this->pos = position;

	// コライダーの追加
	sphere.center = XMLoadFloat3(&pos);
	sphere.center.m128_f32[1] += radius;
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

void StageObject::SetModel(ModelOBJ* obj)
{
	SetModelOBJ(obj);
}

void StageObject::SetModel(ModelFBX* fbx)
{
	SetModelFBX(fbx);
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
	if (info.collider->GetAttribute() == COLLISION_ATTR_ENEMIES && tag != STAGE_OBJECT_DEFAULT)
	{
		Hit(info.collider->attackPower);
		ParticleEmitter::EmitRandomAllRange(3, 60,
			{ info.inter.m128_f32[0], info.inter.m128_f32[1], info.inter.m128_f32[2] },
			{0.0f,0.0f,0.0f},
			{ 1.0f, 0.5f ,0.0f, 1.0f }, { 0.7f, 0.2f, 0.0f, 0.5f },
			0.1f, 0.001f, 1.0f, 0.1);
	}
	if (info.collider->GetAttribute() == COLLISION_ATTR_BULLET + COLLISION_ATTR_ALLIES && tag != STAGE_OBJECT_DEFAULT)
	{
		ParticleEmitter::EmitY_AxisDir(10, 60,
			{ info.inter.m128_f32[0], info.inter.m128_f32[1], info.inter.m128_f32[2] },
			0.0f,
			{ 0.5f, 0.5f ,0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f },
			0.005f, 0.001f, 0.4f, 0.0f);
	}
}

void StageObject::Rejection(const CollisionInfo& info)
{

}