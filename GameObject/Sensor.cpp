#include "Sensor.h"
#include <CollisionAttribute.h>



void Sensor::Initialize(XMFLOAT3 position, float range)
{
	this->pos = position;
	this->range = range;

	Update();
}

void Sensor::Update()
{
	// コライダーの追加
	pos.y += range;
	sensorSphere.center = XMLoadFloat3(&pos);
	sensorColl = new SphereCollider(sensorSphere);
	sensorColl->SetAttribute(COLLISION_ATTR_OBJECT_SENSOR);

	SetCollider(sensorColl);
}

void Sensor::SetSensorRange(float range)
{
	sensorSphere.radius = range;
	sensorColl->SetSphere(sensorSphere);
}

bool Sensor::Detection()
{
	if (CollisionManager::GetInstance()->CheckAllCollision(sensorColl, COLLISION_ATTR_ENEMIES))
	{
		isDetecting = true;
	}
	else
	{
		isDetecting = false;
	}

	return isDetecting;
}

void Sensor::OnCollision(const CollisionInfo& info)
{
	if (info.collider->GetAttribute() == COLLISION_ATTR_ENEMIES)
	{
		if(info.obj) *targetPos = info.obj->GetPosition();
		if(info.fbx) *targetPos = info.fbx->GetPosition();
	}
}
