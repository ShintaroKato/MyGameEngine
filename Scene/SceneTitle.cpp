#include "SceneTitle.h"

SceneTitle::SceneTitle()
{
}

void SceneTitle::Initialize(DirectXCommon* dxCommon, SpriteCommon* sprCommon, Input* input, Audio* audio)
{
	SceneBase::Initialize(dxCommon, sprCommon, input, audio);

	for (int i = 0; i < CUBE_RED_MAX; i++)
	{
		objCubeRed[i]->ObjectOBJ::SetPosition(tmp[i]);
		objCubeGreen[i]->ObjectOBJ::SetPosition(tmp[i + 10]);
		objCubeBlue[i]->ObjectOBJ::SetPosition(tmp[i + 20]);

		objCubeRed[i]->SetUsedFlag(tmpFlag[i]);
		objCubeGreen[i]->SetUsedFlag(tmpFlag[i + 10]);
		objCubeBlue[i]->SetUsedFlag(tmpFlag[i + 20]);

		objCubeRed[i]->PositionFix();
		objCubeGreen[i]->PositionFix();
		objCubeBlue[i]->PositionFix();
	}
	objCastle->ObjectOBJ::SetPosition(tmp[99]);
	objCastle->SetUsedFlag(tmpFlag[99]);
	objCastle->PositionFix();

	objSkydome->SetScale({ 5,5,5 });
	objGroundGrid->ObjectOBJ::SetScale({ 5,5,5 });

	player->SetAllive(false);
	player->SetCameraDistance(80);

	camera->SetTarget(player->GetPosition());
	camera->SetEye({ 0,30,-100 });

	buttonStart->SetPosition({ WinApp::window_width / 2,WinApp::window_height - 256 });
	buttonStart->SetSize({ 128,64 });
	buttonStart->SetAnchorPoint({ 0.5f,0.5f });
	
	buttonEdit->SetPosition({ WinApp::window_width / 2,WinApp::window_height - 256 + 64 + 20 });
	buttonEdit->SetSize({ 128,64 });
	buttonEdit->SetAnchorPoint({ 0.5f,0.5f });

	SceneBase::Update();
}

void SceneTitle::Update()
{
	if (input->TriggerKey(DIK_1) || buttonEdit->Click(MOUSE_LEFT))
	{
		SceneManager::SetScene(EDIT);
	}
	if (input->TriggerKey(DIK_2) || buttonStart->Click(MOUSE_LEFT))
	{
		SceneManager::SetScene(GAME);
	}

	buttonEdit->Update();
	buttonStart->Update();

	SceneBase::Update();
}

void SceneTitle::Draw()
{
#pragma region �w�i�X�v���C�g

	// �X�v���C�g�`��O����
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// �X�v���C�g�`��
	spriteTitle->Draw();

	spriteCommon->PostDraw();

#pragma endregion

#pragma region 3D�I�u�W�F�N�g

	// 3D�I�u�W�F�N�g�`��O����
	// OBJ���f��
	ObjectOBJ::PreDraw(dxCommon->GetCmdList());

	objSkydome->Draw();
	objGroundGrid->ObjectOBJ::Draw();

	for (int i = 0; i < 10; i++)
	{
		if (objCubeRed[i]->GetUsedFlag()) objCubeRed[i]->ObjectOBJ::Draw();
		if (objCubeGreen[i]->GetUsedFlag()) objCubeGreen[i]->ObjectOBJ::Draw();
		if (objCubeBlue[i]->GetUsedFlag()) objCubeBlue[i]->ObjectOBJ::Draw();
	}

	objCastle->ObjectOBJ::Draw();

	objWall->Draw();

	ObjectOBJ::PostDraw();

	// FBX���f��
	//player->ObjectFBX::Draw(dxCommon->GetCmdList());

#pragma endregion

#pragma region �O�i�X�v���C�g

	// �X�v���C�g�`��O����
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// �X�v���C�g�`��
	spriteTitle->Draw();

	buttonEdit->Draw();
	buttonStart->Draw();

	// �e�L�X�g�`��
	//text->DrawAll(dxCommon->GetCmdList());

	spriteCommon->PostDraw();

#pragma endregion

#pragma endregion �O���t�B�b�N�X�R�}���h
}
