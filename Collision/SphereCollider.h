#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>


/// <summary>
/// 球衝突判定オブジェクト
/// </summary>
class SphereCollider :public BaseCollider, public Sphere
{
private:
	using XMVECTOR = DirectX::XMVECTOR;

public:
	SphereCollider(XMVECTOR offset = { 0,0,0,0 }, float radius = 1.0f, bool squareFlag = false)
		: offset(offset), radius(radius), isSquareEnabled(squareFlag)
	{
		// 球形状をセット
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
		// 球形状をセット
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

	// XZ平面に平行な面を作る
	void CreateSquareXZ();

	// 面の各辺の長さを設定
	void SetSquareSideLength(float length_X, float length_Y)
	{
		sqrSideLength = { length_X, length_Y };
	}

	inline const XMFLOAT2& GetSquareSideLength(){ return sqrSideLength; }

	// 球の半径と面が持つ辺の長さの差を取得
	inline const XMFLOAT2& GetSquareSideLengthDifference()
	{
		difference = {
			abs(sqrSideLength.x - radius),
			abs(sqrSideLength.y - radius)
		};

		return difference;
	}

private:
	// オブジェクト中心からのオフセット
	XMVECTOR offset;
	// 半径
	float radius;
	// 球の半径と面が持つ辺の長さの差
	XMFLOAT2 difference;
	// 面が持つ辺の長さ
	XMFLOAT2 sqrSideLength;
	// 面を持たせるか否か
	bool isSquareEnabled = false;
};
