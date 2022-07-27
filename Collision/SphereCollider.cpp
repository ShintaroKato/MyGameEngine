#include "SphereCollider.h"

using namespace DirectX;

void SphereCollider::Update()
{
	// ワールド行列から座標を抽出
	XMMATRIX matWorld{};

	if(obj) matWorld = obj->GetMatWorld();
	if(fbx) matWorld = fbx->GetMatWorld();

	// 球のメンバ変数を更新
	Sphere::center = matWorld.r[3] + offset;
	Sphere::radius = radius;
}
