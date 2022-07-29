#include "SceneStageEdit.h"

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

	objCubeRed[0]->SetPosition({ 25,0,50 });
	objCubeGreen[0]->SetPosition({ -25,0,50 });
	objCubeBlue[0]->SetPosition({ 0,0,-25 });

	objCubeRed[1]->SetPosition({ 50,0,25 });
	objCubeGreen[1]->SetPosition({ -50,0,25 });
	objCubeBlue[1]->SetPosition({ 0,0,-50 });


	objSkydome->SetScale({ 5,5,5 });
	objGround->SetScale({ 5,5,5 });

	player->SetAllive(false);
	player->SetCameraDistance(80);

	camera->SetTarget(player->GetPosition());
	camera->SetEye({ 0,50,-100 });
}

void SceneStageEdit::Update()
{
	if (input->TriggerKey(DIK_ESCAPE))
	{
		SceneManager::SetScene(TITLE);

		for (int i = 0; i < OBJECT_MAX / 3; i++)
		{
			SceneBase::tmp[i] = objCubeRed[i]->ObjectOBJ::GetPosition();
			SceneBase::tmp[i + 10] = objCubeGreen[i]->ObjectOBJ::GetPosition();
			SceneBase::tmp[i + 20] = objCubeBlue[i]->ObjectOBJ::GetPosition();
		}
	}

	camera->SetTarget(player->GetPosition());

	if (input->TriggerKey(DIK_R))
	{
		objCubeRed[0]->SetPosition({ 25,0,50 });
		objCubeGreen[0]->SetPosition({ -25,0,50 });
		objCubeBlue[0]->SetPosition({ 0,0,-25 });

		objCubeRed[1]->SetPosition({ 50,0,25 });
		objCubeGreen[1]->SetPosition({ -50,0,25 });
		objCubeBlue[1]->SetPosition({ 0,0,-50 });
	}

	SceneBase::Update();
}

void SceneStageEdit::Draw()
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

	// OBJ���f��
	ObjectOBJ::PreDraw(dxCommon->GetCmdList());

	//objSphere->Draw();
	objSkydome->Draw();
	//objPlayer->Draw();
	objGround->Draw();

	objCubeRed[0]->ObjectOBJ::Draw();
	objCubeGreen[0]->ObjectOBJ::Draw();
	objCubeBlue[0]->ObjectOBJ::Draw();

	objCubeRed[1]->ObjectOBJ::Draw();
	objCubeGreen[1]->ObjectOBJ::Draw();
	objCubeBlue[1]->ObjectOBJ::Draw();

	ObjectOBJ::PostDraw();

	// FBX���f��
	//player->ObjectFBX::Draw(dxCommon->GetCmdList());

#pragma endregion

#pragma region �O�i�X�v���C�g

	// �X�v���C�g�`��O����
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// �X�v���C�g�`��
	//spriteBG->Draw();
	// �e�L�X�g�`��
	text->DrawAll();

	spriteCommon->PostDraw();


#pragma endregion

#pragma endregion �O���t�B�b�N�X�R�}���h
}

void SceneStageEdit::SaveDat()
{
	//FILE* file = fopen("Resources/Data/stageData.dat", "wb");
	//assert(!file);

	//fwrite()
}
