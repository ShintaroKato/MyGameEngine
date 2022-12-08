#pragma once
#include "Input.h"
#include "ObjectFBX.h"
#include "ObjectOBJ.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include "MeshCollider.h"
#include "StageObject.h"

class StageObjectOffensive : public StageObject
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