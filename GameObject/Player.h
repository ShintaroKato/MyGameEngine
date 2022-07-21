#pragma once
#include "ObjectFBX.h"
#include "ObjectOBJ.h"

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

	/// <summary>
	/// ���W���擾
	/// </summary>
	XMFLOAT3 GetPosition();

	//BaseCollider* GetCollider() { return collider; }

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	//void OnCollision(const CollisionInfo& info) override;

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

private:

	// HP
	float HP = 20.0f;
	const float HPMax = HP;
	float HPRate = HP / HPMax;

	bool alliveFlag = true;

	// �U��
	bool attackFlag = false;
};