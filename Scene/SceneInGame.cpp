#include "SceneInGame.h"

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
		objCubeRed[i]->ObjectOBJ::SetPosition(tmp[i]);
		objCubeGreen[i]->ObjectOBJ::SetPosition(tmp[i + 10]);
		objCubeBlue[i]->ObjectOBJ::SetPosition(tmp[i + 20]);

		objCubeRed[i]->PositionFix();
		objCubeGreen[i]->PositionFix();
		objCubeBlue[i]->PositionFix();
	}
	objCastle->ObjectOBJ::SetPosition(tmp[99]);
	objCastle->PositionFix();

	objSkydome->SetScale({ 5,5,5 });
	objGround->ObjectOBJ::SetScale({ 5,5,5 });

	player->SetPosition({ 0, 0, 0 });
	player->Update();

	camera->SetEye({ 0,2,-5 });
	camera->SetTarget(player->GetPosition());

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		enemy[i]->SetTargetPos(objCastle->GetPosition());
		enemy[i]->SetInGame(true);
	}

	SceneBase::Update();
}

void SceneInGame::Update()
{
	if (input->TriggerKey(DIK_ESCAPE))
	{
		SceneManager::SetScene(TITLE);

		for (int i = 0; i < CUBE_RED_MAX; i++)
		{
			tmp[i] = objCubeRed[i]->ObjectOBJ::GetPosition();
			tmp[i + 10] = objCubeGreen[i]->ObjectOBJ::GetPosition();
			tmp[i + 20] = objCubeBlue[i]->ObjectOBJ::GetPosition();
		}
		tmp[99] = objCastle->ObjectOBJ::GetPosition();
	}

	camera->SetTarget(player->GetPosition());

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
	objGround->ObjectOBJ::Draw();

	for (int i = 0; i < 10; i++)
	{
		if (objCubeRed[i]->GetUsedFlag()) objCubeRed[i]->ObjectOBJ::Draw();
		if (objCubeGreen[i]->GetUsedFlag()) objCubeGreen[i]->ObjectOBJ::Draw();
		if (objCubeBlue[i]->GetUsedFlag()) objCubeBlue[i]->ObjectOBJ::Draw();
	}

	objCastle->ObjectOBJ::Draw();

	objWall->Draw();

	player->ObjectOBJ::Draw();
	weapon[0]->ObjectOBJ::Draw();

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		enemy[i]->ObjectOBJ::Draw();
	}

	ObjectOBJ::PostDraw();

	// FBX���f��
	//player->ObjectFBX::Draw(dxCommon->GetCmdList());

#pragma endregion

#pragma region �O�i�X�v���C�g

	// �X�v���C�g�`��O����
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// �X�v���C�g�`��
	//spriteBG->Draw();

	spriteCommon->PostDraw();

	// �e�L�X�g�`��
	//text->DrawAll(dxCommon->GetCmdList());

#pragma endregion

#pragma endregion �O���t�B�b�N�X�R�}���h
}
