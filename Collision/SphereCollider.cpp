#include "SphereCollider.h"

using namespace DirectX;

void SphereCollider::Update()
{
	// ���[���h�s�񂩂���W�𒊏o
	XMMATRIX matWorld{};

	if (obj) matWorld = obj->GetMatWorld();
	if (fbx) matWorld = fbx->GetMatWorld();

	// ���̃����o�ϐ����X�V
	Sphere::center = offset;
	Sphere::radius = radius;

	CreateSquareXZ();
}

void SphereCollider::CreateSquareXZ()
{
	square = {
		offset, // ���S
		offset.m128_f32[2] + radius, // ��[
		offset.m128_f32[2] - radius, // ���[
		offset.m128_f32[0] - radius, // ���[
		offset.m128_f32[0] + radius  // �E�[
	};
}
