#pragma once
#include "ObjectFBX.h"

class Player : public ObjectFBX
{
public:
	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static Player* Create(ModelFBX* model = nullptr);

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
	/// �ړ��E�W�����v
	/// </summary>
	void Move();

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	/// <summary>
	/// �G�Ƃ̓����蔻��
	/// </summary>
	bool Hit();

	//BaseCollider* GetCollider() { return collider; }

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	//void OnCollision(const CollisionInfo& info) override;

private:
	// HP
	float HP = 20.0f;
	const float HPMax = HP;
	float HPRate = HP / HPMax;

	bool alliveFlag = true;

	// �U��
	bool attackFlag = false;
};