#include "PostEffectScene.h"

PostEffectScene::PostEffectScene()
{
}

PostEffectScene::~PostEffectScene()
{
}

void PostEffectScene::Initialize(DirectXCommon* dxCommon, SpriteCommon* spriteCommon, Input* input, Audio* audio)
{
	SceneBase::Initialize(dxCommon, spriteCommon, input, audio);
	// �J��������
	camera = new Camera();
	camera->Initialize(WinApp::window_width, WinApp::window_height);

	//ObjectOBJ::SetCamera(camera);
	ObjectOBJ::SetDevice(dxCommon->GetDev());

	ObjectFBX::SetCamera(camera);
	ObjectFBX::SetDevice(dxCommon->GetDev());

	// �X�v���C�g���ʃe�N�X�`���ǂݍ���
	spriteCommon->LoadTexture(0, "debugfont.png");
	spriteCommon->LoadTexture(100, "loading.png");

	// �X�v���C�g
	spriteBG = Sprite::Create(spriteCommon, 100, { 0,0 }, { 0,0 });
	spriteBG->Update();

	camera->SetTarget({ 0,0,0 });
	camera->SetEye({ 0,0,-90 });
	camera->Update();
}

void PostEffectScene::Update()
{
	camera->Update();

	spriteBG->Update();
}

void PostEffectScene::Draw()
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
	ObjectOBJ::PreDraw(dxCommon->GetCmdList());

	ObjectOBJ::PostDraw();

#pragma endregion

#pragma region �O�i�X�v���C�g

	// �X�v���C�g�`��O����
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// �X�v���C�g�`��
	spriteBG->Draw();

	spriteCommon->PostDraw();

	// �e�L�X�g�`��
	//text->DrawAll(dxCommon->GetCmdList());

#pragma endregion

#pragma endregion �O���t�B�b�N�X�R�}���h
}
