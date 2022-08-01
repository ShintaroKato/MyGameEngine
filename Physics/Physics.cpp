#include "Physics.h"

XMFLOAT3 operator+(XMFLOAT3 lhs, XMVECTOR rhs)
{
	XMFLOAT3 result{};

	result.x = lhs.x + rhs.m128_f32[0];
	result.y = lhs.y + rhs.m128_f32[1];
	result.z = lhs.z + rhs.m128_f32[2];

	return result;
}

XMFLOAT2 operator+=(XMFLOAT2 lhs, XMVECTOR rhs)
{
	XMFLOAT2 result{};

	result.x = lhs.x + rhs.m128_f32[0];
	result.y = lhs.y + rhs.m128_f32[1];

	return result;
}

Physics::Physics()
{
}

Physics::Physics(float mass, XMVECTOR velocity, XMVECTOR acceleration, XMVECTOR force)
{
	this->mass = mass;
	this->v = velocity;
	this->a = acceleration;
	this->force = force;
}

void Physics::SetParam(float mass, XMVECTOR velocity, XMVECTOR acceleration, XMVECTOR force)
{
	this->mass = mass;
	this->v = velocity;
	this->a = acceleration;
	this->force = force;
}

XMFLOAT2 Physics::UniformlyAccelMotion2D(XMFLOAT2 position, bool gravity)
{
	position += v;

	if (gravity)
	{
		a.m128_f32[1] -= GRAVITY;
	}

	v = v + a;
	a = a + force / mass;

	return position;
}

XMFLOAT3 Physics::UniformlyAccelMotion3D(XMFLOAT3 position, bool gravity)
{
	position = position + v;

	if (gravity)
	{
		a.m128_f32[1] -= GRAVITY;
	}

	v = v + a;
	a = a + force / mass;

	return position;
}
