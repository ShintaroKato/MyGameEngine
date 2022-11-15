#pragma once
#include "CollisionManager.h"
#include "ObjectFBX.h"
#include "ObjectOBJ.h"
#include "SphereCollider.h"
#include "GameObject.h"
#include "GameManager.h"

class Enemy : public ObjectFBX, public ObjectOBJ
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
	static Enemy* Create(ModelOBJ* obj = nullptr);

	/// <summary>
	/// FBX���f������
	/// </summary>
	/// <param name="fbx">���f���f�[�^</param>
	/// <param name="animationNumber">�A�j���[�V�����ԍ�</param>
	/// <returns>�C���X�^���X</returns>
	static Enemy* Create(ModelFBX* fbx = nullptr, int animationNumber = 0);

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
	/// �o��
	/// </summary>
	void Spawn();

	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// �����E�W�����v
	/// </summary>
	void Jump();

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	/// <summary>
	/// �G�Ƃ̓����蔻��
	/// </summary>
	void Hit(float attackPower);

	/// <summary>
	/// ���W���擾
	/// </summary>
	XMFLOAT3 GetPosition();

	/// <summary>
	/// �R���C�_�[���擾
	/// </summary>
	BaseCollider* GetCollider() { return sphereColl; }

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

	/// <summary>
	/// �����t���O��ݒ�
	/// </summary>
	void SetAllive(bool flag) { this->aliveFlag = flag; }

	/// <summary>
	/// �U���Ώۂ̍��W��ݒ�
	/// </summary>
	void SetTargetPos(XMFLOAT3 pos) { this->target = pos; }

	/// <summary>
	/// �Q�[�����ł��邩�ۂ���ݒ�
	/// </summary>
	void SetInGame(bool inGame)
	{
		this->isInGame = inGame;
		if (!isInGame) aliveFlag = false;
	}

private:
	// ���W
	XMFLOAT3 pos{};
	// ��]
	XMFLOAT3 rotation{};
	// ���a
	float radius = 1.0f;
	// �ړ��x�N�g��
	XMVECTOR move = { 0,0,0.1f,0 };
	// �W�I�̍��W
	XMFLOAT3 target{};
	// �R���C�_�[
	Sphere sphere{};
	SphereCollider* sphereColl = nullptr;

	// HP
	float HP = 20.0f;
	const float HPMax = HP;
	float HPRate = HP / HPMax;

	bool aliveFlag = false;

	// �U��
	bool attackFlag = false;

	// �Q�[�����ł��邩�ۂ�
	bool isInGame = false;
};