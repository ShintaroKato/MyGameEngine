#include "SceneInGame.h"

SceneInGame::SceneInGame()
{
}

SceneInGame::~SceneInGame()
{

}

void SceneInGame::Initialize(DirectXCommon* dxCommon, SpriteCommon* sprCommon, Input* input, Audio* audio)
{
	SceneBase::Initialize(dxCommon, sprCommon, input, audio);

	objSkydome->SetScale({ 5,5,5 });
	objGround->SetScale({ 5,5,5 });
	objSphere->SetPosition({ 0,0,-40 });

	camera->SetEye({ 0,500,-50 });
	camera->SetTarget({ 0,0,0 });

	physics = new Physics(10, { 0,0,0.1f }, { 0,0,0.1f }, { 0,0,0 });

	SceneBase::Update();
}

void SceneInGame::Update()
{
	if (input->PushKey(DIK_SPACE))
	{
		objSphere->SetPosition(physics->UniformlyAccelMotion3D(objSphere->GetPosition(), true));
	}
	else
	{
		physics->SetParam(10, { 0,10,5 }, { 0,2.0f,0.1f });
	}

	if (input->TriggerKey(DIK_R))
	{
		objSphere->SetPosition({ 0,0,-40 });
	}

	camera->SetEye({ 0,10,-200 });
	camera->SetTarget({ 0,10,0 });

	SceneBase::Update();
}

void SceneInGame::Draw()
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

	objSphere->Draw();
	objSkydome->Draw();
	objGround->Draw();

	ObjectOBJ::PostDraw();

	// FBX���f��

#pragma endregion

#pragma region �O�i�X�v���C�g

	// �X�v���C�g�`��O����
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// �X�v���C�g�`��
	//spriteBG->Draw();
	// �e�L�X�g�`��
	text->DrawAll(dxCommon->GetCmdList());

	spriteCommon->PostDraw();


#pragma endregion

#pragma endregion �O���t�B�b�N�X�R�}���h
}
