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

	/// <summary>
	/// �������x�^��
	/// </summary>
	void UniformlyAccelMotion();

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