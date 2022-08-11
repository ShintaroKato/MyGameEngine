#pragma once
#include "Input.h"
#include "ObjectFBX.h"
#include "ObjectOBJ.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include "MeshCollider.h"

class GameObject : public ObjectFBX, public ObjectOBJ
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static GameObject* Create(ModelOBJ* obj = nullptr);
	static GameObject* Create(ModelFBX* fbx = nullptr);

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize() override;

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update() override;

	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// �G�Ƃ̓����蔻��
	/// </summary>
	bool Hit();

	/// <summary>
	/// �I�u�W�F�N�g��͂�
	/// </summary>
	void Drag();

	/// <summary>
	/// ���W���Œ肳�ꂽ��Ԃɂ���
	/// </summary>
	void PositionFix() { fix = true; }

	BaseCollider* GetCollider() { return meshColl; }

	XMFLOAT3 GetPosition();

	void SetPosition(const XMFLOAT3& pos);

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	void OnCollision(const CollisionInfo& info) override;

private:
	// ���W
	XMFLOAT3 pos{};
	// ���a
	float radius = 5.0f;
	// �͂܂�Ă��邩�ۂ�
	bool isDrag = false;
	// �ʒu�Œ�
	bool fix = false;
	// �R���C�_�[
	Sphere sphere{};
	MeshCollider* meshColl = nullptr;
};