#pragma once
#include <DirectXMath.h>
#include "ObjectOBJ.h"
#include "ObjectFBX.h"
#define GRAVITY 9.81f / 60 //�d�͉����x(m / frame^2)

using namespace DirectX;

class Physics
{
public: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	Physics();
	Physics(float mass, XMVECTOR velocity, XMVECTOR acceleration, XMVECTOR force);

	void SetParam(float mass, XMVECTOR velocity = { 0,0,0 }, XMVECTOR acceleration = { 0,0,0 }, XMVECTOR force = {0,0,0});

	void ResetParam();

	/// <summary>
	/// �������x�^��(2D)
	/// </summary>
	XMFLOAT2 UniformlyAccelMotion2D(XMFLOAT2 position, bool gravity = false);

	/// <summary>
	/// �������x�^��(3D)
	/// </summary>
	XMFLOAT3 UniformlyAccelMotion3D(XMFLOAT3 position, bool gravity = false);

private:
	// ����
	float m;
	// ���x
	XMVECTOR v0;
	XMVECTOR v;
	// �����x
	XMVECTOR a0;
	XMVECTOR a;
	// ��
	XMVECTOR F;
};