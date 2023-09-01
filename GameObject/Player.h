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

	enum JumpState
	{
		STAY_IN_AIR,
		ON_GROUND,
		WALL_JUMP_BEFORE,
		WALL_JUMP_AFTER,
	};

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
	/// �`��
	/// </summary>
	void Draw() override;

private:
	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// �����E�W�����v
	/// </summary>
	void Jump();

	/// <summary>
	/// �������ɃX�e�b�v
	/// </summary>
	void Step();

	/// <summary>
	/// �ǂ���̉����o��
	/// </summary>
	void Rejection();

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	/// <summary>
	/// �U���G�t�F�N�g
	/// </summary>
	void AttackEffect();

	/// <summary>
	/// �G�Ƃ̓����蔻��
	/// </summary>
	bool Hit();

public:
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

	void SetCamera(Camera* camera)
	{
		this->camera = camera;
		if (ObjectOBJ::model) ObjectOBJ::SetCamera(camera);
		if (ObjectFBX::model) ObjectFBX::SetCamera(camera);
	}

	/// <summary>
	/// �����t���O��ݒ�
	/// </summary>
	void SetAlive(bool flag) { this->aliveFlag = flag; }

	/// <summary>
	/// �Q�[���{�҂��ۂ���ݒ�
	/// </summary>
	void SetInGameFlag(bool flag)
	{
		isInGame = flag;
	}

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

	/// <summary>
	/// �o�����̉��o
	/// </summary>
	bool SpawnAnimation();

	void AnimationTimerReset() { animTimer = 0; }

private:
	// ���W
	XMFLOAT3 pos{};
	// ��]
	XMFLOAT3 rot{};
	// ���a
	float radius = 1.0f;
	// �W�����v���
	JumpState jumpState = ON_GROUND;
	// �����o���t���O
	bool rejectX = false;
	bool rejectZ = false;
	// �ړ��x�N�g��(�����l)
	XMVECTOR move_default = { 0,0,0.3f,0 };
	// �ړ��x�N�g��
	XMVECTOR move = move_default;
	// �U�����̈ړ����x
	float attackMove = 0.4f;
	// �����x�N�g��
	DirectX::XMVECTOR fallVel;
	// �X�e�b�v��������
	bool isStepped = false;
	int stepTimerMax = 30;
	int stepTimer = stepTimerMax;
	int stepEnd = 10;
	XMFLOAT3 stepStartPos{};
	XMFLOAT3 stepEndPos{};
	// �ǂɓ������Ă��邩�ۂ�
	bool hitWall = false;

	// �R���C�_�[
	Sphere sphere{};
	SphereCollider* sphereColl = nullptr;

	// �J����
	Camera* camera = nullptr;

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

	// �o�����̃A�j���[�V�����p�^�C�}�[
	float animTimer = 0;
	float animTimerMax = 120;
	bool isAnimated = false;
};