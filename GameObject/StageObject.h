#pragma once
#include "Input.h"
#include "ObjectFBX.h"
#include "ObjectOBJ.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include "MeshCollider.h"
#include "Sensor.h"

enum UsedState
{
	UNUSED,
	WAITING,
	USED
};

enum Tag
{
	STAGE_OBJECT_DEFAULT,
	RED_OBJECT,
	GREEN_OBJECT,
	BLUE_OBJECT,
	CASTLE_OBJECT,
	OFFENCE_OBJECT,
};

class StageObject : public ObjectFBX, public ObjectOBJ
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
	static StageObject* Create(ModelOBJ* obj = nullptr);
	static StageObject* Create(ModelFBX* fbx = nullptr);

public:
	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	StageObject* GetInstance() { return this; };
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
	/// �G�Ƃ̓����蔻��
	/// </summary>
	void Hit(float attackPower);

	/// <summary>
	/// �I�u�W�F�N�g��͂�
	/// </summary>
	void Drag();

	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// ��]
	/// </summary>
	void Rotation();

	/// <summary>
	/// �f�t�H���g�̏�ԂɕύX
	/// </summary>
	void ChangeDefault();

	/// <summary>
	/// �I�u�W�F�N�g�̃^�C�v
	/// </summary>
	void ObjectType();

	/// <summary>
	/// �e�𔭎�
	/// </summary>
	void Attack();

	/// <summary>
	/// �Q�[���{�҂��ۂ���ݒ�
	/// </summary>
	void SetInGameFlag(bool isInGame)
	{
		this->isInGame = isInGame;
	}

	BaseCollider* GetCollider() { return meshColl; }

	XMFLOAT3 GetPosition();

	void SetPosition(const XMFLOAT3& position);

	UsedState GetUsedState() { return used; }
	void SetUsedState(UsedState state)
	{
		if(!isDrag) used = state;
		SetPosition(pos);
	}

	void SetRadius(float radius) { this->radius = radius; }
	void SetSquareSideLength(float length_X, float length_Y)
	{
		this->sphereColl->SetSquareSideLength(length_X, length_Y);
	}

	void SetModel(ModelOBJ* obj = nullptr);
	void SetModel(ModelFBX* fbx = nullptr);

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	void OnCollision(const CollisionInfo& info) override;

	/// <summary>
	/// ����StageObject�Əd�Ȃ�̂�h��
	/// </summary>
	void Rejection(const CollisionInfo& info);

	/// <summary>
	/// �͂܂�Ă��邩�ۂ��̔���
	/// </summary>
	bool GetDragFlag() { return isDrag; }

	void ChangeDragFlagFalse()
	{
		isDrag = false;
		isDragStatic = false;
	}

	/// <summary>
	/// �^�O���擾
	/// </summary>
	Tag GetTag() { return tag; }

	/// <summary>
	/// �^�O���Z�b�g
	/// </summary>
	void SetTag(Tag tag)
	{
		this->tag = tag;
		ObjectType();
	}

	int GetNumber() { return number; }

	void SetNumber(int number) { this->number = number; }

	void SetAlive(bool aliveFlag) { this->aliveFlag = aliveFlag; }

	/// <summary>
	/// �ϋv�l��ݒ�
	/// </summary>
	/// <returns></returns>
	void SetHP(float HP)
	{
		HPMax = HP;
		this->HP = HPMax;
	}

	/// <summary>
	/// ���݂̑ϋv�l���擾
	/// </summary>
	/// <returns></returns>
	float GetHP() { return HP; }

	/// <summary>
	/// �ϋv�l�̍ő�l���擾
	/// </summary>
	/// <returns></returns>
	float GetHPMax() { return HPMax; }

	void ResetStatus()
	{
		HP = HPMax;
		aliveFlag = true;
	}

protected:
	// ���W
	XMFLOAT3 pos{};
	XMFLOAT3 rot{};
	// ���a
	float radius = 3.5f;
	// �͂܂�Ă��邩�ۂ�
	bool isDrag = false;
	static bool isDragStatic;
	// �g�p����Ă��邩�𔻕�
	UsedState used = UNUSED;
	// �Q�[���{�҂��ۂ�
	bool isInGame = false;
	// �R���C�_�[
	Sphere sphere{};
	MeshCollider* meshColl = nullptr;
	SphereCollider* sphereColl = nullptr;
	// �I�u�W�F�N�g�̎�ނ𔻕ʂ��邽�߂̃^�O
	Tag tag = STAGE_OBJECT_DEFAULT;
	// �ԍ�
	int number = -1;

	// �Փ˂��Ă��邩�ۂ�
	bool hit = false;

	// �U����
	float power = 100.0f;
	// �U���̊Ԋu
	int attackWaitCountMax = 100.0f;
	int attackWaitCount = attackWaitCountMax;
	// �G�����m����Z���T�[
	Sensor* sensor{};

	// �ϋv�l
	float HPMax = 100.0f;
	float HP = HPMax;
	float HPRate = HP / HPMax;

	bool aliveFlag = true;
};

bool operator<(StageObject l, StageObject r);