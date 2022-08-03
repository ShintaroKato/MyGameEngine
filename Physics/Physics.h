#pragma once
#include <DirectXMath.h>
#include "ObjectOBJ.h"
#include "ObjectFBX.h"
#define GRAVITY 9.81f / 60 //重力加速度(m / frame^2)

using namespace DirectX;

class Physics
{
public: // エイリアス
	// DirectX::を省略
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	Physics();
	Physics(float mass, XMVECTOR velocity, XMVECTOR acceleration, XMVECTOR force);

	void SetParam(float mass, XMVECTOR velocity = { 0,0,0 }, XMVECTOR acceleration = { 0,0,0 }, XMVECTOR force = {0,0,0});

	void ResetParam();

	/// <summary>
	/// 等加速度運動(2D)
	/// </summary>
	XMFLOAT2 UniformlyAccelMotion2D(XMFLOAT2 position, bool gravity = false);

	/// <summary>
	/// 等加速度運動(3D)
	/// </summary>
	XMFLOAT3 UniformlyAccelMotion3D(XMFLOAT3 position, bool gravity = false);

private:
	// 質量
	float m;
	// 速度
	XMVECTOR v0;
	XMVECTOR v;
	// 加速度
	XMVECTOR a0;
	XMVECTOR a;
	// 力
	XMVECTOR F;
};