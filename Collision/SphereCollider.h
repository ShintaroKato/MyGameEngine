#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>


/// <summary>
/// ���Փ˔���I�u�W�F�N�g
/// </summary>
class SphereCollider :public BaseCollider, public Sphere
{
private:
	using XMVECTOR = DirectX::XMVECTOR;

public:
	SphereCollider(XMVECTOR offset = { 0,0,0,0 }, float radius = 1.0f, bool squareFlag = false)
		: offset(offset), radius(radius), isSquareEnabled(squareFlag)
	{
		// ���`����Z�b�g
		if (!isSquareEnabled)
		{
			shapeType = COLLISIONSHAPE_SPHERE;
		}
		else
		{
			shapeType = COLLISIONSHAPE_SQUARE;
			CreateSquareXZ();
		}
	}

	SphereCollider(Sphere sphere, bool squareFlag = false)
		: offset(sphere.center), radius(sphere.radius), isSquareEnabled(squareFlag)
	{
		// ���`����Z�b�g
		if (!isSquareEnabled)
		{
			shapeType = COLLISIONSHAPE_SPHERE;
		}
		else
		{
			shapeType = COLLISIONSHAPE_SQUARE;
			CreateSquareXZ();
		}
	}

	void Update() override;

	inline void SetRadius(float radius) { this->radius = radius; }

	inline const XMVECTOR& GetOffset() { return offset; }

	inline void SetOffset(const XMVECTOR& offset) { this->offset = offset; }

	inline float GetRadius() { return radius; }

	inline void SetSphere(Sphere sphere)
	{
		this->offset = sphere.center;
		this->radius = sphere.radius;
	}

	// XZ���ʂɕ��s�Ȗʂ����
	void CreateSquareXZ();

private:
	// �I�u�W�F�N�g���S����̃I�t�Z�b�g
	XMVECTOR offset;
	// ���a
	float radius;
	// �ʂ��������邩�ۂ�
	bool isSquareEnabled = false;
	// ��

};
