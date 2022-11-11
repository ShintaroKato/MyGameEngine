#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct Sphere
{
	XMVECTOR center = { 0,0,0,1 };///中心座標
	float radius = 1.0f;//半径

	struct Square
	{
		XMVECTOR center{};
		float top;
		float bottom;
		float left;
		float right;
	};

	Square square{};
};

struct Plane
{
	XMVECTOR normal = { 0,0,0,1 };//法線ベクトル
	float distance = 0.0f;//原点からの距離
};

struct Ray
{
	// 始点座標
	XMVECTOR start = { 0,0,0,1 };
	// 方向
	XMVECTOR dir = { 1,0,0,0 };
};

class Triangle
{
public:
	// 頂点座標３つ
	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;
	// 法線ベクトル
	DirectX::XMVECTOR normal;

	/// <summary>
	/// 法線の計算
	/// </summary>
	void ComputeNormal();
};