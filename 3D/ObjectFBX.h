#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include "PipelineSet.h"
#include "FBXLoader.h"
#include "Camera.h"
#include "CollisionInfo.h"

class BaseCollider;

/// <summary>
/// 3D�I�u�W�F�N�g
/// </summary>
class ObjectFBX
{
protected: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �萔
	// �{�[���̍ő吔
	static const int MAX_BONES = 64;

public: // �T�u�N���X

	// �萔�o�b�t�@�p�f�[�^�\����B0
	struct ConstBufferDataB0
	{
		XMMATRIX viewProj;	// �r���[�v���W�F�N�V�����s��
		XMMATRIX world;				// ���[���h�s��
		XMFLOAT3 camPos;			// �J�����̈ʒu
	};

	// �萔�o�b�t�@�p�f�[�^�\����(�X�L�j���O)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

public: // �ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="window_width">��ʕ�</param>
	/// <param name="window_height">��ʍ���</param>
	/// <returns>����</returns>
	static bool StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static ObjectFBX* Create();
	/// <summary>
	/// �J�����̐ݒ�
	/// </summary>
	/// <param name="camera"></param>
	static void SetCamera(Camera* camera) { ObjectFBX::camera = camera; }
	/// <summary>
	/// �f�o�C�X�̐ݒ�
	/// </summary>
	/// <param name="device"></param>
	static void SetDevice(ID3D12Device* device) { ObjectFBX::device = device; }

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;
	// �p�C�v���C���Z�b�g
	static PipelineSet pipelineSet;
	// �J����
	static Camera* camera;

private:// �ÓI�����o�֐�

	/// <summary>
	/// �O���t�B�b�N�p�C�v���C������
	/// </summary>
	/// <returns>����</returns>
	static bool InitializeGraphicsPipeline();

public: // �����o�֐�
	virtual bool Initialize();
	/// <summary>
	/// ���t���[������
	/// </summary>
	virtual void Update();

	/// <summary>
	/// ���[���h�s��̍X�V
	/// </summary>
	void UpdateWorldMatrix();

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw();

	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	static void PostDraw();

	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	const XMFLOAT3& GetPosition() { return position; }

	/// <summary>
	/// ��]�̎擾
	/// </summary>
	/// <returns>���W</returns>
	const XMFLOAT3& GetRotation() { return rotation; }

	/// <summary>
	/// ���[���h�s��̎擾
	/// </summary>
	/// <returns>���[���h�s��</returns>
	const XMMATRIX& GetMatWorld() { return matWorld; }
	const XMMATRIX& GetInvMatWorld() { return invMatWorld; }

	/// <summary>
	/// �R���C�_�[���擾
	/// </summary>
	BaseCollider* GetCollider() { return collider; }

	/// <summary>
	/// �{�[���̎p���s����擾
	/// </summary>
	/// <param name="name">�{�[���̖��O</param>
	/// <returns>�s��</returns>
	XMMATRIX GetBoneMatrix(std::string name);

	/// <summary>
	/// �{�[���̎p���s����擾
	/// </summary>
	/// <param name="name">�{�[���̔ԍ�</param>
	/// <returns>�s��</returns>
	XMMATRIX GetBoneMatrix(int i);

	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(XMFLOAT3 position) { this->position = position; }

	/// <summary>
	/// ��]�̐ݒ�
	/// </summary>
	/// <param name="rotation">��]</param>
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }

	/// <summary>
	/// �X�P�[���̐ݒ�
	/// </summary>
	/// <param name="scale">�X�P�[��</param>
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	/// <summary>
	/// ���f���̐ݒ�
	/// </summary>
	void SetModelFBX(ModelFBX* model) { this->model = model; }

	/// <summary>
	/// �A�j���[�V�����Z�b�g
	/// </summary>
	/// <param name="number">�A�j���[�V�����ԍ�</param>
	void SetAnimationNumber(int number);

	/// <summary>
	/// �A�j���[�V�����Đ�
	/// </summary>
	void AnimationPlay();

	/// <summary>
	/// �A�j���[�V�����ꎞ��~
	/// </summary>
	void AnimationStop();

	/// <summary>
	/// �A�j���[�V���������Z�b�g���Ē�~
	/// </summary>
	void AnimationReset();

	/// <summary>
	/// �R���C�_�[�̐ݒ�
	/// </summary>
	void SetCollider(BaseCollider* collider);

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	virtual void OnCollision(const CollisionInfo& info) {}

private: // �����o�ϐ�
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB0;
	// �萔�o�b�t�@(�X�L��)
	ComPtr<ID3D12Resource> constBufferSkin;
	// �A�j���[�V�������ԏ��
	FbxTakeInfo* takeInfo = nullptr;
	// 1�t���[���̎���
	FbxTime frameTime;
	// �A�j���[�V�����J�n����
	FbxTime startTime;
	// �A�j���[�V�����I������
	FbxTime endTime;
	// ���ݎ���
	FbxTime currentTime;

protected: // �����o�ϐ�
	// ���f��
	ModelFBX* model = nullptr;
	// �F
	XMFLOAT4 constColor = { 1,1,1,1 };
	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	// ���[�J�����W
	XMFLOAT3 position = { 0,0,0 };
	// ���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld{};
	XMMATRIX invMatWorld{};
	// �e�I�u�W�F�N�g
	ObjectFBX* parent = nullptr;
	// �A�j���[�V�����Đ���
	bool isPlay = false;
	// �R���C�_�[
	BaseCollider* collider = nullptr;

protected:
	Camera* GetCamera() { return ObjectFBX::camera; }
};