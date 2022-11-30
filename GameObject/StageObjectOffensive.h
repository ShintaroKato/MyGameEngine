#pragma once
#include "Input.h"
#include "ObjectFBX.h"
#include "ObjectOBJ.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include "MeshCollider.h"

class StageObjectOffensive : public ObjectFBX, public ObjectOBJ
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
	static StageObjectOffensive* Create(ModelOBJ* obj = nullptr);
	static StageObjectOffensive* Create(ModelFBX* fbx = nullptr);

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
	/// �G�Ƃ̓����蔻��
	/// </summary>
	void Hit(float attackPower);

	/// <summary>
	/// ���W���Œ肳�ꂽ��Ԃɂ���
	/// </summary>
	void PositionFix()
	{
		isInGame = true;
	}

	BaseCollider* GetCollider() { return meshColl; }

	XMFLOAT3 GetPosition();

	bool GetUsedState() { return used; }
	void SetUsedState(bool flag)
	{
		used = flag;
		SetPosition(pos);
	}

	void SetPosition(const XMFLOAT3& position);

	void SetRadius(float radius) { this->radius = radius; }

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
	/// �^�O���擾
	/// </summary>
	std::string GetTag() { return tag; }

	/// <summary>
	/// �^�O���Z�b�g
	/// </summary>
	void SetTag(std::string tag) { this->tag = tag; }

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

private:
	// ���W
	XMFLOAT3 pos{};
	XMFLOAT3 posTmp{};
	// ���a
	float radius = 4.0f;
	// �g�p����Ă��邩�ۂ�
	bool used = false;
	// �Q�[���{�҂��ۂ�
	bool isInGame = false;
	// �R���C�_�[
	Sphere sphere{};
	MeshCollider* meshColl = nullptr;
	// �I�u�W�F�N�g�̎�ނ𔻕ʂ��邽�߂̃^�O
	std::string tag = "default";

	// �Փ˂��Ă��邩�ۂ�
	bool hit = false;

	// �ϋv�l
	float HPMax = 100.0f;
	float HP = HPMax;
	float HPRate = HP / HPMax;

	bool aliveFlag = true;
};