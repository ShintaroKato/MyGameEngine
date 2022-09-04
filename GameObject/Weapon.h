#pragma once
#include "CollisionManager.h"
#include "ObjectFBX.h"
#include "ObjectOBJ.h"
#include "SphereCollider.h"
#include "GameObject.h"


class Weapon : public ObjectFBX, public ObjectOBJ
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
	static Weapon* Create(ModelOBJ* obj = nullptr);

	/// <summary>
	/// FBX���f������
	/// </summary>
	/// <param name="fbx">���f���f�[�^</param>
	/// <param name="animationNumber">�A�j���[�V�����ԍ�</param>
	/// <returns>�C���X�^���X</returns>
	static Weapon* Create(ModelFBX* fbx = nullptr, int animationNumber = 0);

	void SetParent(ObjectOBJ* obj) { this->ObjectOBJ::parent = obj; }
	void SetParent(ObjectFBX* fbx) { this->ObjectFBX::parent = fbx; }

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

private:
	// ���W
	XMFLOAT3 pos{};
	// ��]
	XMFLOAT3 rotation{};
	// ���a
	float radius = 1.0f;
	// �ڒn�t���O
	bool onGround = true;
	// �ړ��x�N�g��
	XMVECTOR move = { 0,0,0.1f,0 };
	// �����x�N�g��
	DirectX::XMVECTOR fallVel;
	// �R���C�_�[
	Sphere sphere{};
	SphereCollider* sphereColl = nullptr;
	// �U���p�R���C�_�[
	Sphere attackSphere{};
};
