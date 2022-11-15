#pragma once
#include "CollisionPrimitive.h"

class Collision
{
public:
	/// <summary>
	/// 当たり判定（球と球）
	/// </summary>
	/// <param name="sphere1">球１</param>
	/// <param name="sphere2">球２</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckShpere2Sphere(const Sphere& sphere1, const Sphere& sphere2, XMVECTOR* inter = nullptr, XMVECTOR* reject = nullptr);	/// <summary>

	/// 当たり判定（矩形と矩形）
	/// </summary>
	/// <param name="sphere1">球１</param>
	/// <param name="sphere2">球２</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSquare2Square(const Sphere::Square& square1, const Sphere::Square& square2, XMVECTOR* inter = nullptr, XMVECTOR* reject = nullptr);

	/// <summary>
	/// 当たり判定（球と平面）
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="plane">平面</param>
	/// <param name="inter">交点（平面上の最近接点）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckShpere2Plane(const Sphere& sphere, const Plane& plane, XMVECTOR* inter = nullptr);

	/// <summary>
	/// 点と三角形の最近接点を求める
	/// </summary>
	/// <param name="point">点</param>
	/// <param name="triangle">三角形</param>
	/// <param name="closest">最近接点（出力用）</param>
	static void ClosestPtPoint2Triangle(const XMVECTOR& point,
		const Triangle& triangle, XMVECTOR* closest);

	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle,
		XMVECTOR* inter = nullptr, XMVECTOR* reject = nullptr);

	/// <summary>
	/// レイと平面の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="plane">平面</param>
	/// <param name="distance">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool ChackRay2Plane(const Ray& ray, const Plane& plane,
		float* distance = nullptr, XMVECTOR* inter = nullptr);

	/// <summary>
	/// レイと法線付き三角形の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="triangle">三角形</param>
	/// <param name="distance">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle,
		float* distance = nullptr, XMVECTOR* inter = nullptr);

	/// <summary>
	/// レイと球の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="sphere">球</param>
	/// <param name="distance">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool ChackRay2Sphere(const Ray& ray, const Sphere& sphere,
		float* distance = nullptr, XMVECTOR* inter = nullptr);
};