#include "Sensor.h"
#include <CollisionAttribute.h>



void Sensor::Initialize()
{
	sensorColl->SetSphere(sensorSphere);
}

void Sensor::SetSensorRange(float range)
{
	sensorSphere.radius = range;
	sensorColl->SetSphere(sensorSphere);
}

void Sensor::Detection()
{
	if (CollisionManager::GetInstance()->CheckAllCollision(sensorColl, COLLISION_ATTR_ENEMIES))
	{
		isDetecting = true;
	}
	else
	{
		isDetecting = false;
	}
}

void Sensor::OnCollision(const CollisionInfo& info)
{
	if (info.collider->GetAttribute() == COLLISION_ATTR_ENEMIES)
	{
		if(info.obj) targetPos = info.obj->GetPosition();
		if(info.fbx) targetPos = info.fbx->GetPosition();
	}
}
