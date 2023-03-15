#include "Sensor.h"
#include <CollisionAttribute.h>



Sensor::Sensor() {}

Sensor::Sensor(XMFLOAT3 position, float range)
{
	this->pos = position;
	this->range = range;

	// コライダーの追加
	sensorSphere.center = XMLoadFloat3(&pos);
	sensorSphere.radius = range;
	sensorColl = new SphereCollider(sensorSphere);
	sensorColl->SetAttribute(COLLISION_ATTR_OBJECT_SENSOR);

	SetCollider(sensorColl);
}

void Sensor::Update()
{
	// コライダーの追加
	sensorSphere.center = XMLoadFloat3(&pos);
	sensorSphere.radius = range;
	sensorColl->SetSphere(sensorSphere);
	sensorColl->SetAttribute(COLLISION_ATTR_OBJECT_SENSOR);

	SetCollider(sensorColl);

	Detection();
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
		if(info.obj) targetPos = info.obj->GetPosition();
		if(info.fbx) targetPos = info.fbx->GetPosition();
	}
}

void Sensor::Delete()
{
	CollisionManager::GetInstance()->RemoveCollider(sensorColl);
}
