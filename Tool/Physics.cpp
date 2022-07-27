#include "Physics.h"

XMFLOAT3 operator+(XMFLOAT3 lhs, XMVECTOR rhs)
{
	XMFLOAT3 result{};

	result.x = lhs.x + rhs.m128_f32[0];
	result.y = lhs.y + rhs.m128_f32[1];
	result.z = lhs.z + rhs.m128_f32[2];

	return result;
}

Physics::Physics()
{
}

Physics::Physics(float mass, XMVECTOR velocity, XMVECTOR acceleration)
{
	this->mass = mass;
	this->velocity = velocity;
	this->acceleration = acceleration;
}

void Physics::Update()
{
	if (obj)
	{
		obj->SetPosition(position);
		obj->Update();
	}
	if (fbx)
	{
		fbx->SetPosition(position);
		fbx->Update();
	}
}

void Physics::SetObjectOBJ(ObjectOBJ* obj)
{
	this->obj = obj;

	position = obj->GetPosition();
}

void Physics::SetObjectFBX(ObjectFBX* fbx)
{
	this->fbx = fbx;

	position = fbx->GetPosition();
}

void Physics::UniformlyAccelMotion()
{
	if (obj == nullptr) return;

	position = position + velocity;
	velocity = velocity + acceleration;

	Update();
}

