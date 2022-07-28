#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

/// <summary>
/// ���b�V���Փ˔���I�u�W�F�N�g
/// </summary>
class MeshCollider : public BaseCollider
{
public:
	MeshCollider()
	{
		//���b�V���`������Z�b�g
		shapeType = COLLISIONSHAPE_MESH;
	}

	/// <summary>
	/// �O�p�`�̔z����\�z����
	/// </summary>
	void ConstructTriangle(ModelOBJ* model);
	void ConstructTriangle(ModelFBX* model);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// ���Ƃ̓����蔻��
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="inter">��_�i�o�͗p�j</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	bool CheckCollisionSphere(const Sphere& sphere,
		DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// ���C�Ƃ̓����蔻��
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="distance">����</param>
	/// <param name="inter">��_�i�o�͗p�j</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	bool CheckCollisionRay(const Ray& ray, float* distance = nullptr,
		DirectX::XMVECTOR* inter = nullptr);

private:
	std::vector<Triangle>triangles;
	// ���[���h�s��̋t�s��
	DirectX::XMMATRIX invMatWorld;
};