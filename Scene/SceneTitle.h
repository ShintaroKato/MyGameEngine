#pragma once
#include "SceneBase.h"

/// <summary>
/// �Q�[���V�[��
/// </summary>
class SceneTitle : public SceneBase
{
public: // �����o�֐�

	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	SceneTitle();

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

private:
	bool startUnlock = false;
};