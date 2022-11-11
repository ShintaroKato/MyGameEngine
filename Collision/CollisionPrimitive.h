#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct Sphere
{
	XMVECTOR center = { 0,0,0,1 };///���S���W
	float radius = 1.0f;//���a

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
	XMVECTOR normal = { 0,0,0,1 };//�@���x�N�g��
	float distance = 0.0f;//���_����̋���
};

struct Ray
{
	// �n�_���W
	XMVECTOR start = { 0,0,0,1 };
	// ����
	XMVECTOR dir = { 1,0,0,0 };
};

class Triangle
{
public:
	// ���_���W�R��
	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;
	// �@���x�N�g��
	DirectX::XMVECTOR normal;

	/// <summary>
	/// �@���̌v�Z
	/// </summary>
	void ComputeNormal();
};