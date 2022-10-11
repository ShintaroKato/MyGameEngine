#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

/// <summary>
/// メッシュ衝突判定オブジェクト
/// </summary>
class MeshCollider : public BaseCollider
{
public:
	MeshCollider()
	{
		//メッシュ形状をリセット
		shapeType = COLLISIONSHAPE_MESH;
	}

	/// <summary>
	/// 三角形の配列を構築する
	/// </summary>
	void ConstructTriangle(ModelOBJ* model);
	void ConstructTriangle(ModelFBX* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 球との当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	bool CheckCollisionSphere(const Sphere& sphere,
		DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// レイとの当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="distance">距離</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	bool CheckCollisionRay(const Ray& ray, float* distance = nullptr,
		DirectX::XMVECTOR* inter = nullptr);

private:
	ModelOBJ* obj = nullptr;
	ModelFBX* fbx = nullptr;
	std::vector<Triangle>triangles;
	// ワールド行列の逆行列
	DirectX::XMMATRIX matWorld;
	DirectX::XMMATRIX invMatWorld;
};