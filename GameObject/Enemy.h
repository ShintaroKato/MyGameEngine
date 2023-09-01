#pragma once
#include "CollisionManager.h"
#include "ObjectFBX.h"
#include "ObjectOBJ.h"
#include "SphereCollider.h"
#include "StageObject.h"
#include "Weapon.h"

enum EnemyType
{
	STRAIGHT,
	FLYING,
	ROUTE_RANDOM,
	ROUTE_SEARCH,
	TARGET_PLAYER,
	TYPE_COUNT
};

enum ObjectType
{
	NONE_OBJ,
	OBSTACLE,
	TARGET_OBJ
};

struct NaviNode
{
	XMFLOAT3 pos;	// ���W
	float costG;	// �X�^�[�g����S�[���܂ł̃R�X�g
	float costH;	// �S�[���܂ł̐���R�X�g
	float costF;	// costG - costH
	ObjectType objectType;	// ��Q���̎��
	NaviNode* parent;	// ��O�̃m�[�h�ւ̃|�C���^

	NaviNode(XMFLOAT3 pos)
	{
		this->pos = pos;
		costG = 0;
		costH = 0;
		costF = 0;
		objectType = NONE_OBJ;
		parent = nullptr;
	}
};

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
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// �ړ�
	/// </summary>
	void MoveRouteSearch();

	/// <summary>
	/// ��s
	/// </summary>
	void Fly();

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	/// <summary>
	/// �U��
	/// </summary>
	void AttackBeam();

	/// <summary>
	/// �G�Ƃ̓����蔻��
	/// </summary>
	void Hit(float attackPower);

	/// <summary>
	/// ��e���̔���
	/// </summary>
	void HitReaction();

	/// <summary>
	/// �|���ꂽ���̏���
	/// </summary>
	void Defeated();

	/// <summary>
	/// �o�����̃A�j���[�V����
	/// </summary>
	void SpawnAnimation();

	/// <summary>
	/// ���W���擾
	/// </summary>
	XMFLOAT3 GetPosition();

	/// <summary>
	/// �R���C�_�[���擾
	/// </summary>
	BaseCollider* GetCollider() { return sphereColl; }

	// �����t���O���擾
	bool GetAliveFlag() { return aliveFlag; }

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
	/// �\�͒l��ݒ�
	/// </summary>
	/// <param name="HP">�̗�</param>
	/// <param name="speed">����(�{���Őݒ�)</param>
	/// <param name="power">�U����</param>
	/// <param name="radius">���R���C�_�[�̔��a�̒���</param>
	void SetStatus(float HP, float speed, float power, float radius);

	/// <summary>
	/// �U���Ώۂ̃I�u�W�F�N�g��ݒ�
	/// </summary>
	void SetTargetPos(StageObject* object)
	{
		this->target = object;
		SetTargetPos(target->GetPosition());
	}

	/// <summary>
	/// �U���Ώۂ̃I�u�W�F�N�g�̈ʒu��ݒ�
	/// </summary>
	void SetTargetPos(XMFLOAT3 pos)
	{
		targetPos = pos;
		// �W�I�̕���������
		rot.y = XMConvertToDegrees(
			atan2f(targetPos.x - this->pos.x, targetPos.z - this->pos.z));
	}

	/// <summary>
	/// �^�C�v��ݒ�
	/// </summary>
	void SetType(EnemyType enemyType);

	/// <summary>
	/// ������Z�b�g
	/// </summary>
	void SetWeapon(Weapon* weapon);

	/// <summary>
	/// �o�����̃A�j���[�V�������L�����ۂ���ݒ�
	/// </summary>
	void SetSpawnAnimationActiveFlag(bool flag) { spawnAnimActive = flag; }

	/// <summary>
	/// ������擾
	/// </summary>
	Weapon* GetWeapon() { return weapon; }

	/// <summary>
	/// �U���͂��擾
	/// </summary>
	float GetPower() { return attackPower + weapon->GetPower(); }

	/// <summary>
	/// �^�C�v���擾
	/// </summary>
	EnemyType GetType() { return type; }

	/// <summary>
	/// �|���ꂽ���ۂ����擾
	/// </summary>
	bool GetDefeatFlag() { return defeated; }

public:
	/// <summary>
	/// �o�H�T���p�̃��b�V�����Z�b�g
	/// </summary>
	/// <param name="mesh">���b�V���̃|���S���̃f�[�^</param>
	static void CreateNaviMesh(std::vector<Triangle> mesh);

	static std::vector<Triangle> GetNaviMesh() { return naviMesh; }

private:

	// �q���[���X�e�B�b�N�֐�(���[�N���b�h����)
	float Heuristic(XMFLOAT3 pos, XMFLOAT3 nextpos)
	{
		float res = (
			(pos.x - nextpos.x) * (pos.x - nextpos.x) +
			(pos.y - nextpos.y) * (pos.y - nextpos.y) +
			(pos.z - nextpos.z) * (pos.z - nextpos.z));

		return sqrtf(res);
	}

	static std::vector<NaviNode> naviNode;
	// �o�H�T���p���b�V�����\������S�Ẵ|���S���̃f�[�^�̔z��
	static std::vector<Triangle> naviMesh;

private:
	// ���W
	XMFLOAT3 pos{};
	// ��]
	XMFLOAT3 rot{};
	// ���a
	float radius = 1.0f;
	// �ړ��x�N�g��
	XMVECTOR move = { 0,0,0.1f,0 };
	XMVECTOR moveDefault = { 0,0,0.1f,0 };
	// �W�I�̃I�u�W�F�N�g
	StageObject* target{};
	// �W�I�̍��W
	XMFLOAT3 targetPos{};
	// �R���C�_�[
	Sphere sphere{};
	SphereCollider* sphereColl = nullptr;

	// �^�C�v
	EnemyType type = STRAIGHT;

	// HP
	float HP = 100.0f;
	float HPMax = HP;
	float HPRate = HP / HPMax;

	// �����t���O
	bool aliveFlag = false;

	// ���j�t���O
	bool defeated = false;

	// ���G����
	int noDamageTime = 0;
	int noDamageTimeMax = 10;

	// �U��
	Weapon* weapon = nullptr;
	bool attackFlag = false;
	float attackPower = 0.1f;

	// �U���̊Ԋu
	int attackCountMax = 600.0f;
	int attackCount = attackCountMax;

	bool isStop = false;

	// �o�����̃A�j���[�V����
	bool spawnAnimActive = false;

	// �G�t�F�N�g�p�^�C�}�[
	int effectTimer = 10;
	XMFLOAT3 effectPos{};
};