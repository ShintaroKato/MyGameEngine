#pragma once
#include "SceneBase.h"
#include "SceneManager.h"
#include "GameManager.h"

/// <summary>
/// �Q�[���V�[��
/// </summary>
class SceneInGame : public SceneBase
{
public: // �����o�֐�

	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	SceneInGame();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~SceneInGame();

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
	/// ���j���[�\��
	/// </summary>
	void Menu();

private:
	bool menuON = false;
};