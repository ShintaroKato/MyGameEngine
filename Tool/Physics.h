#pragma once
#include <DirectXMath.h>
#include "ObjectOBJ.h"
#include "ObjectFBX.h"

using namespace DirectX;

class Physics
{
public: // �G�C���A�X
	// DirectX::���ȗ�
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
	/// �������x�^��(2D)
	/// </summary>
	void UniformlyAccelMotion2D();

	/// <summary>
	/// �������x�^��(3D)
	/// </summary>
	void UniformlyAccelMotion3D();

private:
	// �I�u�W�F�N�g
	ObjectOBJ* obj = nullptr;
	ObjectFBX* fbx = nullptr;
	// �ʒu
	XMFLOAT3 position;
	// ����
	float mass;
	// ���x
	XMVECTOR velocity;
	// �����x
	XMVECTOR acceleration;
};