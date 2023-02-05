#include "SceneTitle.h"
#include "SceneManager.h"

SceneTitle::SceneTitle()
{
}

void SceneTitle::Initialize(DirectXCommon* dxCommon, SpriteCommon* sprCommon, Input* input, Audio* audio)
{
	SceneBase::Initialize(dxCommon, sprCommon, input, audio);

	//for (int i = 0; i < objTmp.size(); i++)
	//{
	//	SceneBase::LoadStage(objTmp[i]);
	//}

	objSkydome->SetScale({ 5,5,5 });
	objGroundGrid->ObjectOBJ::SetScale({ 5,5,5 });

	player->SetAllive(false);
	player->SetInGameFlag(false);
	player->SetCameraDistance(80);

	camera->SetTarget(player->GetPosition());
	camera->SetEye({ 0,30,-100 });

	buttonStart->SetPosition({ WinApp::window_width / 2,WinApp::window_height - 256 });
	buttonStart->SetSize({ 128,64 });
	buttonStart->SetAnchorPoint({ 0.5f,0.5f });

	for (int i = 0; i < stgObjects.size(); i++)
	{
		if (stgObjects[i]->GetTag() == CASTLE_OBJECT) startUnlock = true;
	}

	buttonEdit->SetPosition({ WinApp::window_width / 2,WinApp::window_height - 256 + 64 + 20 });
	buttonEdit->SetSize({ 128,64 });
	buttonEdit->SetAnchorPoint({ 0.5f,0.5f });

	SceneBase::Update();
}

void SceneTitle::Update()
{
	spriteCursor->SetPosition(input->GetMousePos2());

	if (input->TriggerKey(DIK_1) || buttonEdit->Click(MOUSE_LEFT))
	{
		SceneManager::SetScene(EDIT);

		return;
	}
	if (startUnlock &&
		(input->TriggerKey(DIK_2) || buttonStart->Click(MOUSE_LEFT)))
	{
		SceneManager::SetScene(GAME);

		return;
	}

	if (input->PushMouse(MOUSE_RIGHT))
	{
		player->SetCameraMoveFlag(true);
	}
	else
	{
		player->SetCameraMoveFlag(false);
	}

	buttonEdit->Update();

	if (startUnlock) buttonStart->Update();

	spriteCursor->Update();

	SceneBase::Update();

	for (int i = 0; i < stgObjects.size(); i++)
	{
		if (stgObjects[i]->GetUsedState() != UNUSED) stgObjects[i]->Update();
	}

	SortObjectCameraDistance();
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
	objSkydomeSpace->Draw();
	objGroundGrid->ObjectOBJ::Draw();

	objWall->Draw();

	for (int i = 0; i < stgObjects.size(); i++)
	{
		if (stgObjects[i]->GetUsedState() != UNUSED) stgObjects[i]->ObjectOBJ::Draw();
	}

	ObjectOBJ::PostDraw();

	particle->Draw(dxCommon->GetCmdList());

	// FBX���f��
	//player->ObjectFBX::Draw(dxCommon->GetCmdList());

#pragma endregion

#pragma region �O�i�X�v���C�g

	// �X�v���C�g�`��O����
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	buttonEdit->Draw();

	if (startUnlock) buttonStart->Draw();

	// �X�v���C�g�`��
	spriteTitle->Draw();
	spriteCursor->Draw();
	// �e�L�X�g�`��
	//text->DrawAll(dxCommon->GetCmdList());

	spriteCommon->PostDraw();

#pragma endregion

#pragma endregion �O���t�B�b�N�X�R�}���h
}
