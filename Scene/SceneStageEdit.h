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
	/// ���j���[�\���X�V
	/// </summary>
	void MenuUpdate();

	/// <summary>
	/// ���j���[�\���`��
	/// </summary>
	void MenuDraw();

	/// <summary>
	/// �I�u�W�F�N�g�쐬
	/// </summary>
	bool MakeObject(StageObject* stgObject, Button* button, ModelOBJ* model, const Tag& objectTag, const XMFLOAT2& sideLength = { 3.5f, 3.5f });
	bool MakeObject(StageObject* stgObject, Button* button, ModelFBX* model, const Tag& objectTag, const XMFLOAT2& sideLength = { 3.5f, 3.5f });

	/// <summary>
	/// dat�t�@�C���ɕۑ�
	/// </summary>
	void SaveDat();

private:
	StageObject* stgObjectEdit[OBJECT_MAX];

	bool menuON = true;
	bool buttonClick = false;
};