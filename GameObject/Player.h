#pragma once
#include "CollisionManager.h"
#include "ObjectFBX.h"
#include "ObjectOBJ.h"
#include "SphereCollider.h"
#include "StageObject.h"
#include "Weapon.h"

class Player : public ObjectFBX, public ObjectOBJ
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
	static Player* Create(ModelOBJ* obj = nullptr);

	/// <summary>
	/// FBX���f������
	/// </summary>
	/// <param name="fbx">���f���f�[�^</param>
	/// <param name="animationNumber">�A�j���[�V�����ԍ�</param>
	/// <returns>�C���X�^���X</returns>
	static Player* Create(ModelFBX* fbx = nullptr, int animationNumber = 0);

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
	/// �����E�W�����v
	/// </summary>
	void Jump();

	/// <summary>
	/// ���_����
	/// </summary>
	void ControlCamera();

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	/// <summary>
	/// �G�Ƃ̓����蔻��
	/// </summary>
	bool Hit();

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

	void Rejection(const CollisionInfo& info);

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
	/// �v���C���[�ƃJ�����̋���
	/// </summary>
	void SetCameraDistance(float distance) { this->distance = distance; }

	/// <summary>
	/// �����t���O��ݒ�
	/// </summary>
	void SetAllive(bool flag) { this->aliveFlag = flag; }

	/// <summary>
	/// �Q�[���{�҂��ۂ���ݒ�
	/// </summary>
	void SetInGameFlag(bool flag)
	{
		isInGame = flag;
		cameraRotY = 0;
		cameraRotX = 30;
	}

	void SetCameraMoveFlag(bool flag) { cameraMoveActive = flag; }

	///// <summary>
	///// ������Z�b�g
	///// </summary>
	void SetWeapon(Weapon* weapon);

	/// <summary>
	/// ������擾
	/// </summary>
	Weapon* GetWeapon() { return weapon; }

	/// <summary>
	/// �U���͂��擾
	/// </summary>
	float GetPower() { return power + weapon->GetPower(); }

	float GetHP() { return HP; }
	float GetHPMax()
	{
		float max = HPMax;
		return max;
	}


private:
	// ���W
	XMFLOAT3 pos{};
	// ��]
	XMFLOAT3 rot{};
	// ���a
	float radius = 1.0f;
	// �ڒn�t���O
	bool onGround = true;
	// �ړ��x�N�g��(�����l)
	XMVECTOR move_default = { 0,0,0.2f,0 };
	// �ړ��x�N�g��
	XMVECTOR move{};
	// �U�����̈ړ����x
	float attackMove = 0.4f;
	// �����x�N�g��
	DirectX::XMVECTOR fallVel;
	// �R���C�_�[
	Sphere sphere{};
	SphereCollider* sphereColl = nullptr;

	// �J����
	XMFLOAT3 cameraPos{};
	XMFLOAT3 cameraRot{};
	float cameraRotX = 0;
	float cameraRotY = 0;
	float distance = 10;
	// �J��������ON�EOFF
	bool cameraMoveActive = true;

	// HP
	const float HPMax = 200.0f;
	float HP = HPMax;
	float HPRate = HP / HPMax;

	bool aliveFlag = true;

	// �U��
	Weapon* weapon = nullptr;
	bool attackFlag = false;
	int attackLevel = 0;
	float attackCount = 0;
	float power = 10.0f;

	// �Q�[���{�҂��ۂ�
	bool isInGame = true;
};