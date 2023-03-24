#pragma once
#include "SceneBase.h"

/// <summary>
/// �Q�[���V�[��
/// </summary>
class SceneStageEdit : public SceneBase
{
public: // �����o�֐�

	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	SceneStageEdit();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~SceneStageEdit();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(DirectXCommon* dxCommon, SpriteCommon* sprCommon, Input* input, Audio* audio)override;

	// 2D�I�u�W�F�N�g�̏�����
	void Initialize2D();
	// 3D�I�u�W�F�N�g�̏�����
	void Initialize3D();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update()override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw()override;

	/// <summary>
	/// ���j���[�\���X�V
	/// </summary>
	void UpdateMenu();

	/// <summary>
	/// ���j���[�\���`��
	/// </summary>
	void DrawMenu();

	/// <summary>
	/// �I�u�W�F�N�g�쐬(OBJ)
	/// </summary>
	bool MakeObject(Button* button, ModelOBJ* model, const Tag& objectTag, const XMFLOAT2& sideLength = { 3.5f, 3.5f });
	/// <summary>
	/// �I�u�W�F�N�g�쐬(FBX)
	/// </summary>
	bool MakeObject(Button* button, ModelFBX* model, const Tag& objectTag, const XMFLOAT2& sideLength = { 3.5f, 3.5f });

	/// <summary>
	/// �X�e�[�W�I�u�W�F�N�g���J��������̋����ŕ��בւ���
	/// (�߂��ɂ���قǔz��̏�ɓ���)
	/// </summary>
	void SortObjectCameraDistance();

	/// <summary>
	/// dat�t�@�C���ɕۑ�
	/// </summary>
	void SaveDat();

private:
	std::forward_list<StageObject> stgObjectEdit;

	bool menuActivate = true;
	bool buttonClick = false;
	bool windowActive = false;
	bool listActive = false;
};