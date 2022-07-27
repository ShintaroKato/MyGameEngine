#include "SphereCollider.h"

using namespace DirectX;

void SphereCollider::Update()
{
	// ���[���h�s�񂩂���W�𒊏o
	XMMATRIX matWorld{};

	if(obj) matWorld = obj->GetMatWorld();
	if(fbx) matWorld = fbx->GetMatWorld();

	// ���̃����o�ϐ����X�V
	Sphere::center = matWorld.r[3] + offset;
	Sphere::radius = radius;
}
