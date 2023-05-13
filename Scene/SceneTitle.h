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

	/// <summary>
	/// ��ԂŃJ�����𓮂���(�V�[���؂�ւ����̉��o)
	/// </summary>
	void CameraAnimation();

private:
	// �J��������p
	XMFLOAT3 startPos{};
	float nowCount = 0;
	float maxCount = 30;
	size_t startIndex = 1;
};