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
	if(tag == UNUSED)

	if (HP <= 0) aliveFlag = false;

	if (!isInGame)
	{
		ResetStatus();
		Drag();
		Move();
		Rotation();
		ChangeDefault();

	}
	else if (aliveFlag)
	{
		if(tag == STAGE_OBJECT_OFFENCE)
		{
			sensor->SetPosition(pos);
			sensor->Update();
			Attack();
		}

		XMFLOAT3 effectPos = pos;
		effectPos.x += (float)(rand() % 10 - 5);
		effectPos.y += (float)(rand() % 10 - 5);
		effectPos.z += (float)(rand() % 10 - 5);

		for (int i = 0; i < 2; i++)
		{
			if (HP <= HPMax / 2)
			{
				ParticleEmitter::Spark(16, 8, 4, effectPos, effectPos, 0.4f,
					{ 1.0f,0.0f,0.0f,1.0f }, { 1.0f,1.0f,0.5f,1.0f },
					radius / 2, 0.0f, 0.0f, 0.2f, 0.2f);
			}
			if (HP <= HPMax / 4)
			{
				ParticleEmitter::Spark(16, 8, 4, effectPos, effectPos, 0.4f,
					{ 1.0f,0.2f,0.2f,1.0f }, { 1.0f,0.0f,0.0f,1.0f },
					radius / 2, 0.0f, 0.0f, 0.2f, 0.2f);
			}
		}
	}

	SetPosition(pos);

	ObjectOBJ::Update();
	ObjectFBX::Update();

	if(meshColl) meshColl->Update();
	if(sphereColl) sphereColl->Update();
}

void StageObject::Draw()
{
	if(tag == UNUSED)
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

	XMVECTOR vec = input->CursorPoint3D(Camera::GetViewMatrix(), Camera::GetProjectionMatrix());
	Ray ray{};
	ray.start = XMLoadFloat3(&Camera::GetEye());
	ray.dir = XMVector3Normalize(vec - ray.start);

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
}

void StageObject::Move()
{
	float range = PlaneCursor::GetMovableRange();
	if (pos.x < -range) pos.x = -range;
	if (pos.x > range) pos.x = range;

	if (pos.z < -range) pos.z = -range;
	if (pos.z > range) pos.z = range;

	if (!isDrag) pos.y = 0;
}

void StageObject::Rotation()
{
	Input* input = Input::GetInstance();

	if (!isDrag) return;

	if(input->PushKey(DIK_LSHIFT) || input->PushKey(DIK_RSHIFT))
	{
		if (input->TriggerKey(DIK_A))
		{
			if ((int)rot.y % 90 != 0) rot.y -= (int)rot.y % 90;
			else					  rot.y -= 90.0f;
		}
		else if (input->TriggerKey(DIK_D))
		{
			if ((int)rot.y % 90 != 0) rot.y += 90 - (int)rot.y % 90;
			else					  rot.y += 90.0f;
		}
	}
	else
	{
		if (input->PushKey(DIK_A))		rot.y -= 2.0f;
		else if (input->PushKey(DIK_D))	rot.y += 2.0f;
	}

	if (rot.y >= 360) rot.y -= 360;
	if (rot.y <= -360) rot.y += 360;

	if (ObjectOBJ::model) ObjectOBJ::SetRotation(rot);
	if (ObjectFBX::model) ObjectFBX::SetRotation(rot);
}

void StageObject::ChangeDefault()
{
	if (!isDrag || tag == STAGE_OBJECT_CASTLE) return;

	Input* input = Input::GetInstance();

	if (input->TriggerKey(DIK_SPACE))
	{
		isDrag = false;
		isDragStatic = false;
		PlaneCursor::SetIsDrag(isDragStatic);

		if (sensor)
		{
			sensor->Delete();
			delete sensor;
		}

		tag = STAGE_OBJECT_DEFAULT;
		used = UNUSED;
	}
}

void StageObject::ObjectType()
{
	switch (tag)
	{
	case STAGE_OBJECT_CASTLE:
		radius = 5.0f;
		HPMax = 500;
		HP = HPMax;
		break;

	case STAGE_OBJECT_WALL:
		radius = 5.0f;
		HPMax = 1200;
		HP = HPMax;
		break;

	case STAGE_OBJECT_BUILDING:
		radius = 4.0f;
		HPMax = 100;
		HP = HPMax;
		break;

	case BLUE_OBJECT:
		radius = 5.0f;
		HPMax = 800;
		HP = HPMax;
		break;

	case STAGE_OBJECT_OFFENCE:
		sensor = new Sensor(pos, 32);
		radius = 4.0f;
		HPMax = 100;
		HP = HPMax;
		power = 100.0f;
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
	if(sensor->Detection())
	{
		XMFLOAT3 target = sensor->GetTargetPos();
		XMFLOAT3 cannonPos = pos;
		cannonPos.y += 10;

		if (attackWaitCount > 0)
		{
			attackWaitCount--;
		}
		else
		{

			XMFLOAT3 dist{};
			dist.x = target.x - cannonPos.x;
			dist.y = target.y - cannonPos.y;
			dist.z = target.z - cannonPos.z;

			XMFLOAT3 cannonRot{};
			cannonRot.y = XMConvertToDegrees(atan2f(dist.x, dist.z));
			cannonRot.x = -XMConvertToDegrees(atan2f(dist.y, sqrtf(dist.x * dist.x + dist.z * dist.z)));

			BulletManager::GetInstance()->Fire(cannonPos, cannonRot, COLLISION_ATTR_ALLIES, meshColl, 3.0f, power);
			attackWaitCount = attackWaitCountMax;

		}

		Ray ray;
		ray.start = XMLoadFloat3(&cannonPos);
		ray.dir = XMLoadFloat3(&target);
		ray.dir = XMVector3Normalize(ray.dir - ray.start);
		RaycastHit raycast{};
		XMFLOAT3 inter{};

		CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_ALLIES, meshColl, &raycast);

		XMFLOAT3 ePos{};
		if(raycast.collider->GetAttribute() == COLLISION_ATTR_ENEMIES)
		{
			if (raycast.obj) ePos = raycast.obj->GetPosition();
			if (raycast.fbx) ePos = raycast.fbx->GetPosition();
			inter = ePos;
		}
		else
		{
			inter.x = raycast.inter.m128_f32[0];
			inter.y = raycast.inter.m128_f32[1];
			inter.z = raycast.inter.m128_f32[2];
		}

		ParticleEmitter::Spark(32, 8, 4, inter, cannonPos, -0.1f,
			{ 0.0f,0.0f,1.0f,1.0f }, { 0.0f,0.1f,0.1f,1.0f }, 1.2f, 0.0f, 0.0f, 0.5f, 0.5f);
		ParticleEmitter::LaserBeam(16, 8, cannonPos, inter, 0.5f,
			{ 0.2f,0.2f,1.0f,0.01f }, { 0.2f,0.2f,1.0f,0.01f }, 0.0f, 0.01f, 2.0f, 2.0f);
	}
	else
	{
		attackWaitCount = attackWaitCountMax;
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
	if (info.collider->GetAttribute() == COLLISION_ATTR_ENEMIES && tag != STAGE_OBJECT_DEFAULT ||
		info.collider->GetAttribute() == COLLISION_ATTR_ENEMIES + COLLISION_ATTR_BULLET && tag != STAGE_OBJECT_DEFAULT)
	{
		Hit(info.collider->attackPower);
		ParticleEmitter::EmitRandomAllRange(3, 60,
			{ info.inter.m128_f32[0], info.inter.m128_f32[1], info.inter.m128_f32[2] },
			{ 0.0f,0.0f,0.0f },
			{ 1.0f, 0.5f ,0.0f, 1.0f }, { 0.7f, 0.2f, 0.0f, 0.5f },
			0.1f, 0.001f, 1.0f, 0.1);
	}
	if (info.collider->GetAttribute() == COLLISION_ATTR_BULLET + COLLISION_ATTR_ALLIES && tag != STAGE_OBJECT_DEFAULT)
	{
		ParticleEmitter::EmitY_AxisDir(10, 60,
			{ info.inter.m128_f32[0], info.inter.m128_f32[1], info.inter.m128_f32[2] },
			0.01f,
			{ 0.5f, 0.5f ,0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f },
			0.005f, 0.001f, 0.4f, 0.0f);
	}
}

void StageObject::Rejection(const CollisionInfo& info)
{

}

bool operator<(StageObject l, StageObject r)
{
	return l.GetCameraDistance() >= r.GetCameraDistance();
}
