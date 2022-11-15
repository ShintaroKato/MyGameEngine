#include "SceneInGame.h"
#include "SceneManager.h"

SceneInGame::SceneInGame()
{
}

SceneInGame::~SceneInGame()
{
	delete player;
}

void SceneInGame::Initialize(DirectXCommon* dxCommon, SpriteCommon* sprCommon, Input* input, Audio* audio)
{
	SceneBase::Initialize(dxCommon, sprCommon, input, audio);

	for (int i = 0; i < CUBE_RED_MAX; i++)
	{
		SceneBase::LoadStage(objCubeRed[i], true);
		SceneBase::LoadStage(objCubeGreen[i], true);
		SceneBase::LoadStage(objCubeBlue[i], true);
	}
	SceneBase::LoadStage(objCastle, true);

	objSkydome->SetScale({ 5,5,5 });
	objGroundGrid->ObjectOBJ::SetScale({ 5,5,5 });

	player->SetPosition({ 0, 0, 0 });
	player->Update();

	camera->SetTarget(player->GetPosition());
	camera->SetEye({ 0,2,5 });

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		enemy[i]->SetTargetPos(objCastle->GetPosition());
		enemy[i]->SetInGame(true);
		enemy[i]->SetAllive(false);
	}

	menuON = false;

	buttonTitle->SetPosition({ 0, 0 });
	buttonTitle->SetSize({ 128,64 });

	meterPlayerHP->SetPosition({ 0, WinApp::window_height - 64 });
	meterPlayerHP->SetSize(
		{ 320, 64 },
		{ 320, 64 },
		{ 320, 64 });
	meterPlayerHP->SetValue(player->GetHP(), player->GetHPMax());

	spriteWaveClear->SetPosition({ WinApp::window_width / 2, 128 });

	GameManager::Start();

	SceneBase::Update();
}

void SceneInGame::Update()
{
	Menu();

	if (menuON && buttonTitle->Click(MOUSE_LEFT))
	{
		for (int i = 0; i < CUBE_RED_MAX; i++)
		{
			SceneBase::SaveStage(objCubeRed[i]);
			SceneBase::SaveStage(objCubeGreen[i]);
			SceneBase::SaveStage(objCubeBlue[i]);
		}
		SceneBase::SaveStage(objCastle);

		SceneManager::SetScene(TITLE);

		return;
	}

	camera->SetTarget({
		player->GetPosition().x,
		player->GetPosition().y + 2,
		player->GetPosition().z
		});

	if (menuON) return;

	numberTimer->SetSequence(GameManager::GetTimerSeconds(), 0, 64, { 32,64 });
	numberWaitTimer->SetSequence(GameManager::GetWaitTimerSeconds(), WinApp::window_width / 2, 128, { 32, 64 });

	buttonTitle->Update();

	meterPlayerHP->SetValue(player->GetHP(), player->GetHPMax());
	meterPlayerHP->Update();

	spriteWaveClear->Update();

	GameManager::Update();

	SceneBase::Update();
}

void SceneInGame::Draw()
{
#pragma region �w�i�X�v���C�g

	// �X�v���C�g�`��O����
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// �X�v���C�g�`��
	//spriteBG->Draw();

	spriteCommon->PostDraw();

#pragma endregion

#pragma region 3D�I�u�W�F�N�g

	// 3D�I�u�W�F�N�g�`��O����

	// OBJ���f��
	ObjectOBJ::PreDraw(dxCommon->GetCmdList());

	//objSphere->Draw();
	objSkydome->Draw();
	objGroundGrid->ObjectOBJ::Draw();

	for (int i = 0; i < 10; i++)
	{
		if (objCubeRed[i]->GetUsedFlag()) objCubeRed[i]->ObjectOBJ::Draw();
		if (objCubeGreen[i]->GetUsedFlag()) objCubeGreen[i]->ObjectOBJ::Draw();
		if (objCubeBlue[i]->GetUsedFlag()) objCubeBlue[i]->ObjectOBJ::Draw();
	}

	if(objCastle->GetUsedFlag()) objCastle->ObjectOBJ::Draw();

	objWall->Draw();

	player->ObjectOBJ::Draw();
	weapon[0]->ObjectOBJ::Draw();

	if(GameManager::GetFinishState() == 0)
	{
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			enemy[i]->ObjectOBJ::Draw();
		}
	}

	ObjectOBJ::PostDraw();

	// FBX���f��
	//player->ObjectFBX::Draw(dxCommon->GetCmdList());

#pragma endregion

#pragma region �O�i�X�v���C�g

	// �X�v���C�g�`��O����
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	numberTimer->Draw();
	if(GameManager::GetWaitTimer() > 0) numberWaitTimer->Draw();
	meterPlayerHP->Draw();

	if(menuON)
	{
		buttonTitle->Draw();
	}
	
	// �X�v���C�g�`��
	//spriteBG->Draw();
	if (GameManager::GetFinishState() == 1) spriteWaveClear->Draw();
	if (menuON) spriteCursor->Draw();

	spriteCommon->PostDraw();

	// �e�L�X�g�`��
	//text->DrawAll(dxCommon->GetCmdList());

#pragma endregion

#pragma endregion �O���t�B�b�N�X�R�}���h
}

void SceneInGame::Menu()
{
	if (!menuON)
	{
		// �}�E�X�J�[�\���̍��W����ʒ����ɌŒ�
		SetCursorPos(WinApp::window_width / 2, WinApp::window_height / 2);
		player->SetCameraMoveFlag(true);

		if (input->TriggerKey(DIK_ESCAPE))
		{
			menuON = true;
		}
	}
	else
	{
		spriteCursor->SetPosition(input->GetMousePos2());
		spriteCursor->Update();
		player->SetCameraMoveFlag(false);

		if (input->TriggerKey(DIK_ESCAPE))
		{
			menuON = false;
		}
	}
}
