#include "SceneBase.h"

void SceneBase::Initialize(DirectXCommon* dxCommon, SpriteCommon* spriteCommon, Input* input, Audio* audio)
{
	// nullptr�`�F�b�N
	assert(dxCommon);
	assert(spriteCommon);
	assert(input);

	this->dxCommon = dxCommon;
	this->spriteCommon = spriteCommon;
	this->input = input;
	this->audio = audio;

	// �J��������
	camera = new Camera();
	camera->Initialize(WinApp::window_width, WinApp::window_height);

	ObjectOBJ::SetCamera(camera);
	ObjectOBJ::SetDevice(this->dxCommon->GetDev());

	ObjectFBX::SetCamera(camera);
	ObjectFBX::SetDevice(this->dxCommon->GetDev());

	// �X�v���C�g���ʃe�N�X�`���ǂݍ���
	spriteCommon->LoadTexture(0, "debugfont.png");
	spriteCommon->LoadTexture(1, "background.png");

	// �e�L�X�g
	text = Text::GetInstance();
	text->Initialize(spriteCommon, 0);

	// �X�v���C�g
	spriteBG = Sprite::Create(spriteCommon, 1, { 0,0 }, { 0,0 });
	spriteBG->Update();

	// obj.���烂�f���f�[�^�ǂݍ���
	modelGround = ModelOBJ::LoadObj("ground", true);
	modelSkydome = ModelOBJ::LoadObj("skydome", true);
	modelSphere = ModelOBJ::LoadObj("sphere", true);

	// 3D�I�u�W�F�N�g����
	objGround = ObjectOBJ::Create();
	objSkydome = ObjectOBJ::Create();
	objSphere = ObjectOBJ::Create();
	// �I�u�W�F�N�g�Ƀ��f����R�Â���
	objGround->SetModelOBJ(modelGround);
	objSkydome->SetModelOBJ(modelSkydome);
	objSphere->SetModelOBJ(modelSphere);
}

void SceneBase::Update()
{
	camera->Update();

	objGround->Update();
	objSkydome->Update();
	objSphere->Update();

	spriteBG->Update();
}
