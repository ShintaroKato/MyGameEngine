#include "SceneStageEdit.h"
#include "SceneManager.h"
#include "PlaneCursor.h"

SceneStageEdit::SceneStageEdit()
{
}

SceneStageEdit::~SceneStageEdit()
{
	delete player;
}

void SceneStageEdit::Initialize(DirectXCommon* dxCommon, SpriteCommon* sprCommon, Input* input, Audio* audio)
{
	SceneBase::Initialize(dxCommon, sprCommon, input, audio);

	buttonTitle->SetPosition({ 0,0 });
	buttonTitle->SetSize({ 128,64 });

	buttonRed->SetPosition({ 20,20 + 64 });
	buttonRed->SetSize({ 64,64 });
	buttonRed->Update();

	buttonGreen->SetPosition({ 20,20 + 64 * 2});
	buttonGreen->SetSize({ 64,64 });
	buttonGreen->Update();

	buttonBlue->SetPosition({ 20,20 + 64 * 3 });
	buttonBlue->SetSize({ 64,64 });
	buttonBlue->Update();

	for (int i = 0; i < CUBE_RED_MAX; i++)
	{
		SceneBase::LoadStage(objCubeRed[i]);
		SceneBase::LoadStage(objCubeGreen[i]);
		SceneBase::LoadStage(objCubeBlue[i]);
	}
	SceneBase::LoadStage(objCastle);
	objCastle->SetUsedState(USED);

	PlaneCursor::Initialize(objCursor);

	objSkydome->SetScale({ 5,5,5 });
	objGroundGrid->ObjectOBJ::SetScale({ 5,5,5 });

	player->SetAllive(false);
	player->SetCameraDistance(80);
	player->SetInGameFlag(false);

	camera->SetTarget(player->GetPosition());
	camera->SetEye({ 0,50,-100 });
}

void SceneStageEdit::Update()
{
	spriteCursor->SetPosition(input->GetMousePos2());

	if (input->TriggerKey(DIK_ESCAPE) || buttonTitle->Click(MOUSE_LEFT))
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

	camera->SetTarget(player->GetPosition());
	camera->SetEye({ 0,30,-100 });
	if (input->PushMouse(MOUSE_RIGHT))
	{
		player->SetCameraMoveFlag(true);
	}
	else
	{
		player->SetCameraMoveFlag(false);
	}

	for (int i = 0; i < 10; i++)
	{
		if (buttonRed->Click(MOUSE_LEFT) && !objCubeRed[i]->GetUsedState())
		{
			objCubeRed[i]->SetUsedState(WAITING);
			buttonRed->SetClickFlag(false);
			break;
		}
		if (buttonGreen->Click(MOUSE_LEFT) && !objCubeGreen[i]->GetUsedState())
		{
			objCubeGreen[i]->SetUsedState(WAITING);
			buttonGreen->SetClickFlag(false);
			break;
		}
		if (buttonBlue->Click(MOUSE_LEFT) && !objCubeBlue[i]->GetUsedState())
		{
			objCubeBlue[i]->SetUsedState(WAITING);
			buttonBlue->SetClickFlag(false);
			break;
		}
	}

	buttonTitle->Update();
	buttonRed->Update();
	buttonGreen->Update();
	buttonBlue->Update();
	spriteCursor->Update();

	SceneBase::Update();

	PlaneCursor::Update();
}

void SceneStageEdit::Draw()
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

	objSkydome->Draw();
	objGroundGrid->ObjectOBJ::Draw();

	for (int i = 0; i < 10; i++)
	{
		if (objCubeRed[i]->GetUsedState()) objCubeRed[i]->ObjectOBJ::Draw();
		if (objCubeGreen[i]->GetUsedState()) objCubeGreen[i]->ObjectOBJ::Draw();
		if (objCubeBlue[i]->GetUsedState()) objCubeBlue[i]->ObjectOBJ::Draw();
	}

	objCastle->ObjectOBJ::Draw();

	objWall->Draw();
	PlaneCursor::Draw();
	ObjectOBJ::PostDraw();

	// FBX���f��
	//player->ObjectFBX::Draw(dxCommon->GetCmdList());

#pragma endregion

#pragma region �O�i�X�v���C�g

	// �X�v���C�g�`��O����
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	buttonTitle->Draw();
	buttonRed->Draw();
	buttonGreen->Draw();
	buttonBlue->Draw();
	// �X�v���C�g�`��
	spriteCursor->Draw();
	// �e�L�X�g�`��
	text->DrawAll();

	spriteCommon->PostDraw();


#pragma endregion

#pragma endregion �O���t�B�b�N�X�R�}���h
}
