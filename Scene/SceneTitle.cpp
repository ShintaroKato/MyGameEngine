#include "SceneTitle.h"

SceneTitle::SceneTitle()
{
}

void SceneTitle::Initialize(DirectXCommon* dxCommon, SpriteCommon* sprCommon, Input* input, Audio* audio)
{
	SceneBase::Initialize(dxCommon, sprCommon, input, audio);

	SceneBase::Update();
}

void SceneTitle::Update()
{
	if (input->TriggerKey(DIK_1))
	{
		SceneManager::SetScene(EDIT);
	}
	if (input->TriggerKey(DIK_2))
	{
		SceneManager::SetScene(GAME);
	}

	SceneBase::Update();
}

void SceneTitle::Draw()
{
#pragma region �w�i�X�v���C�g

	// �X�v���C�g�`��O����
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// �X�v���C�g�`��
	spriteBG->Draw();

	spriteCommon->PostDraw();

#pragma endregion

#pragma region 3D�I�u�W�F�N�g

	// 3D�I�u�W�F�N�g�`��O����
	//ObjectOBJ::PreDraw(dxCommon->GetCmdList());

	//objSphere->Draw();

	//ObjectOBJ::PostDraw();

#pragma endregion

#pragma region �O�i�X�v���C�g

	// �X�v���C�g�`��O����
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// �X�v���C�g�`��
	//spriteBG->Draw();

	// �e�L�X�g�`��
	//text->DrawAll(dxCommon->GetCmdList());

	spriteCommon->PostDraw();

#pragma endregion

#pragma endregion �O���t�B�b�N�X�R�}���h
}
