#pragma once
#include "Input.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include "MeshCollider.h"

class Sensor : public ObjectNone
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
	// ������
	void Initialize(XMFLOAT3 position, float range);

	// �X�V
	void Update();

	// �G�����m����͈͂�ݒ�
	void SetSensorRange(float range);

	// �G�����m
	bool Detection();

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	void OnCollision(const CollisionInfo& info);

	// �W�I�̍��W���擾
	XMFLOAT3 GetTargetPos() { return *targetPos; }

private:
	// ���W
	XMFLOAT3 pos;

	// �G�����m����͈�(���̔��a)
	float range;
	Sphere sensorSphere{};
	SphereCollider* sensorColl = nullptr;

	// �W�I�̍��W
	XMFLOAT3* targetPos;

	// ���m���Ă���ۂ�
	bool isDetecting = false;

	// �Փ˂��Ă��邩�ۂ�
	bool hit = false;
};