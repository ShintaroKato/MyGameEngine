#include "SphereCollider.h"

using namespace DirectX;

void SphereCollider::Update()
{
	// ワールド行列から座標を抽出
	XMMATRIX matWorld{};

	if (obj) matWorld = obj->GetMatWorld();
	if (fbx) matWorld = fbx->GetMatWorld();

	// 球のメンバ変数を更新
	Sphere::center = offset;
	Sphere::radius = radius;

	CreateSquareXZ();
}

void SphereCollider::CreateSquareXZ()
{
	square = {
		offset, // 中心
		offset.m128_f32[2] + radius, // 上端
		offset.m128_f32[2] - radius, // 下端
		offset.m128_f32[0] - radius, // 左端
		offset.m128_f32[0] + radius  // 右端
	};
}
