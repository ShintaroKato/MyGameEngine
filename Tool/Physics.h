#pragma once
#include <DirectXMath.h>
#include "ObjectOBJ.h"
#include "ObjectFBX.h"

using namespace DirectX;

class Physics
{
public: // エイリアス
	// DirectX::を省略
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	Physics();
	Physics(float mass, XMVECTOR velocity, XMVECTOR acceleration);

	void Update();

	void SetObjectOBJ(ObjectOBJ* obj);

	void SetObjectFBX(ObjectFBX* fbx);

	void SetParam(float mass, XMVECTOR velocity, XMVECTOR acceleration);

	/// <summary>
	/// 等加速度運動(2D)
	/// </summary>
	void UniformlyAccelMotion2D();

	/// <summary>
	/// 等加速度運動(3D)
	/// </summary>
	void UniformlyAccelMotion3D();

private:
	// オブジェクト
	ObjectOBJ* obj = nullptr;
	ObjectFBX* fbx = nullptr;
	// 位置
	XMFLOAT3 position;
	// 質量
	float mass;
	// 速度
	XMVECTOR velocity;
	// 加速度
	XMVECTOR acceleration;
};