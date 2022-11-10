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
	SphereCollider(XMVECTOR offset = { 0,0,0,0 }, float radius = 1.0f)
		: offset(offset), radius(radius)
	{
		// 球形状をセット
		shapeType = COLLISIONSHAPE_SPHERE;
	}

	SphereCollider(Sphere sphere)
		: offset(sphere.center), radius(sphere.radius)
	{
		// 球形状をセット
		shapeType = COLLISIONSHAPE_SPHERE;
	}

	void Update() override;

	inline void SetRadius(float radius) {
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
					shapeType = COLLISIONSHAPE_SPHERE_SQUARE;
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
					shapeType = COLLISIONSHAPE_SPHERE_SQUARE;
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
			inline void CreateSquareXZ();

		private:
			// オブジェクト中心からのオフセット
			XMVECTOR offset;
			// 半径
			float radius;

			// 面を持たせるか否か
			bool isSquareEnabled = false;
		};

		this->radius = radius; }

	inline const XMVECTOR& GetOffset() { return offset; }

	inline void SetOffset(const XMVECTOR& offset) { this->offset = offset; }

	inline float GetRadius() { return radius; }

	inline void SetSphere(Sphere sphere)
	{
		this->offset = sphere.center;
		this->radius = sphere.radius;
	}

private:
	// オブジェクト中心からのオフセット
	XMVECTOR offset;
	// 半径
	float radius;
};

