#include "TitleScene.h"

TitleScene::TitleScene()
{
}

void TitleScene::Initialize(DirectXCommon* dxCommon, SpriteCommon* sprCommon, Input* input, Audio* audio)
{
	// nullptr�`�F�b�N
	assert(dxCommon);
	assert(sprCommon);
	assert(input);

	this->dxCommon = dxCommon;
	this->spriteCommon = sprCommon;
	this->input = input;

	// �X�v���C�g���ʃe�N�X�`���ǂݍ���
	spriteCommon->LoadTexture(0, L"Resources/debugfont.png");
	spriteCommon->LoadTexture(1, L"Resources/background.png");

	// �e�L�X�g
	text->GetInstance()->Initialize(spriteCommon, 0);
	// �X�v���C�g
	spriteBG = Sprite::Create(spriteCommon, 1, { 0,0 }, { 0,0 });
	spriteBG->Update();

	//// obj.���烂�f���f�[�^�ǂݍ���
	//modelSphere = Model::LoadObj("sphere");
	//// 3D�I�u�W�F�N�g����
	//objSphere = Object3d::Create();
	//// �I�u�W�F�N�g�Ƀ��f����R�Â���
	//objSphere->SetModel(modelSphere);

	//objSphere->SetPosition({ 0,0,30 });
	//objSphere->Update();
}

void TitleScene::Update()
{
	if (input->PushKey(DIK_SPACE))
	{
		SceneManager::SceneChange();
	}

	spriteBG->Update();
}

void TitleScene::Draw()
{
	dxCommon->PreDraw();

#pragma region �w�i�X�v���C�g

	// �X�v���C�g�`��O����
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// �X�v���C�g�`��
	spriteBG->Draw();

	spriteCommon->PostDraw();

#pragma endregion

#pragma region 3D�I�u�W�F�N�g

	// 3D�I�u�W�F�N�g�`��O����
	//Object3d::PreDraw(dxCommon->GetCmdList());

	//objSphere->Draw();

	//Object3d::PostDraw();

#pragma endregion

#pragma region �O�i�X�v���C�g

	// �X�v���C�g�`��O����
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// �X�v���C�g�`��
	spriteBG->Draw();

	// �e�L�X�g�`��
	//text->DrawAll(dxCommon->GetCmdList());

	spriteCommon->PostDraw();

#pragma endregion


	dxCommon->PostDraw();
#pragma endregion �O���t�B�b�N�X�R�}���h
}
