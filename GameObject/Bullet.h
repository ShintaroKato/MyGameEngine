#pragma once
#include "ObjectNone.h"
#include "BaseCollider.h"
#include "SphereCollider.h"
#include "CollisionAttribute.h"

class Bullet : public ObjectNone
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
	/// OBJ���f������
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static Bullet* Create();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	void OnCollision(const CollisionInfo& info) override;

	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	void SetPosition(XMFLOAT3 pos);

	/// <summary>
	/// ��]��ݒ�
	/// </summary>
	void SetRotation(XMFLOAT3 rot);

	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	void SetScale(XMFLOAT3 scale);

	SphereCollider* GetSphereCollider() { return this->sphereColl; };

	// �U���͂��擾
	float GetPower() { return power; }

public:
	// ���W
	XMFLOAT3 position{};
	// ��]
	XMFLOAT3 rotation{};
	// ���a
	float radius = 1.0f;
	// ���x
	XMVECTOR velocity = { 0, 0, 1.0f };
	// �R���C�_�[
	Sphere sphere{};
	SphereCollider* sphereColl = nullptr;
	unsigned short collAttribute = COLLISION_ATTR_BULLET;

	// �U����
	float power = 0.0f;
	// �J�n�t���[��
	int frame = 0;
	// �I���t���[��
	int end_frame = 100;
};
