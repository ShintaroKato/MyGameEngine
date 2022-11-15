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

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update()override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw()override;

	/// <summary>
	/// dat�t�@�C���ɕۑ�
	/// </summary>
	void SaveDat();
};