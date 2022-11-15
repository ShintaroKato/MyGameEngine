#pragma once
#include "CollisionPrimitive.h"

class Collision
{
public:
	/// <summary>
	/// �����蔻��i���Ƌ��j
	/// </summary>
	/// <param name="sphere1">���P</param>
	/// <param name="sphere2">���Q</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckShpere2Sphere(const Sphere& sphere1, const Sphere& sphere2, XMVECTOR* inter = nullptr, XMVECTOR* reject = nullptr);	/// <summary>

	/// �����蔻��i��`�Ƌ�`�j
	/// </summary>
	/// <param name="sphere1">���P</param>
	/// <param name="sphere2">���Q</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckSquare2Square(const Sphere::Square& square1, const Sphere::Square& square2, XMVECTOR* inter = nullptr, XMVECTOR* reject = nullptr);

	/// <summary>
	/// �����蔻��i���ƕ��ʁj
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="plane">����</param>
	/// <param name="inter">��_�i���ʏ�̍ŋߐړ_�j</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckShpere2Plane(const Sphere& sphere, const Plane& plane, XMVECTOR* inter = nullptr);

	/// <summary>
	/// �_�ƎO�p�`�̍ŋߐړ_�����߂�
	/// </summary>
	/// <param name="point">�_</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="closest">�ŋߐړ_�i�o�͗p�j</param>
	static void ClosestPtPoint2Triangle(const XMVECTOR& point,
		const Triangle& triangle, XMVECTOR* closest);

	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle,
		XMVECTOR* inter = nullptr, XMVECTOR* reject = nullptr);

	/// <summary>
	/// ���C�ƕ��ʂ̓����蔻��
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="plane">����</param>
	/// <param name="distance">�����i�o�͗p�j</param>
	/// <param name="inter">��_�i�o�͗p�j</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool ChackRay2Plane(const Ray& ray, const Plane& plane,
		float* distance = nullptr, XMVECTOR* inter = nullptr);

	/// <summary>
	/// ���C�Ɩ@���t���O�p�`�̓����蔻��
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="distance">�����i�o�͗p�j</param>
	/// <param name="inter">��_�i�o�͗p�j</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle,
		float* distance = nullptr, XMVECTOR* inter = nullptr);

	/// <summary>
	/// ���C�Ƌ��̓����蔻��
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="sphere">��</param>
	/// <param name="distance">�����i�o�͗p�j</param>
	/// <param name="inter">��_�i�o�͗p�j</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool ChackRay2Sphere(const Ray& ray, const Sphere& sphere,
		float* distance = nullptr, XMVECTOR* inter = nullptr);
};