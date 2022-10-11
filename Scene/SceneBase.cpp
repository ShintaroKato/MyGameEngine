#include "SceneBase.h"

XMFLOAT3 SceneBase::tmp[OBJECT_MAX];

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
	spriteCommon->LoadTexture(debug_font, "debugfont.png");
	spriteCommon->LoadTexture(title, "title_transparent.png");
	spriteCommon->LoadTexture(button_r, "256x256Red.png");
	spriteCommon->LoadTexture(button_g, "256x256Green.png");
	spriteCommon->LoadTexture(button_b, "256x256Blue.png");

	// �e�L�X�g
	text = Text::GetInstance();
	text->Initialize(spriteCommon, debug_font);

	// �X�v���C�g
	spriteTitle = Sprite::Create(spriteCommon, title, { 0,0 }, { 0,0 });
	buttonRed = Button::Create(spriteCommon, button_r, { 0,0 }, { 0,0 });
	buttonGreen = Button::Create(spriteCommon, button_g, { 0,0 }, { 0,0 });
	buttonBlue = Button::Create(spriteCommon, button_b, { 0,0 }, { 0,0 });

	spriteTitle->Update();

	// .obj���烂�f���f�[�^�ǂݍ���
	modelSkydome = ModelOBJ::LoadObj("skydome");
	modelGround = ModelOBJ::LoadObj("ground");
	modelPlayer = ModelOBJ::LoadObj("chr_sword");
	modelEnemy = ModelOBJ::LoadObj("player");
	modelCubeRed = ModelOBJ::LoadObj("cube64Red");
	modelCubeGreen = ModelOBJ::LoadObj("brokenBlock");
	modelCubeBlue = ModelOBJ::LoadObj("cube64Blue");
	modelCastle = ModelOBJ::LoadObj("small_castle");
	modelWall = ModelOBJ::LoadObj("square_wall");
	modelWeapon = ModelOBJ::LoadObj("sword2");

	// 3D�I�u�W�F�N�g����
	objSkydome = ObjectOBJ::Create();
	// �I�u�W�F�N�g�Ƀ��f����R�Â���
	objSkydome->SetModelOBJ(modelSkydome);
	objSkydome->SetScale({ 5,5,5 });

	objWall = ObjectOBJ::Create();
	objWall->SetModelOBJ(modelWall);
	objWall->SetScale({ 12,1,12 });

	objGround = TouchableObject::Create(modelGround);
	objGround->ObjectOBJ::SetScale({ 5,5,5 });

	for (int i = 0; i < CUBE_RED_MAX; i++)
	{
		objCubeRed[i] = GameObject::Create(modelCubeRed);
		objCubeGreen[i] = GameObject::Create(modelCubeGreen);
		objCubeBlue[i] = GameObject::Create(modelCubeBlue);
	}
	objCastle = GameObject::Create(modelCastle);

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		enemy[i] = Enemy::Create(modelEnemy);
		enemy[i]->SetInGame(false);
		enemy[i]->SetScale({ 2,2,2 });
	}

	weapon[0] = Weapon::Create(modelWeapon);

	player = Player::Create(modelPlayer);
	player->ObjectOBJ::SetCamera(camera);
	player->SetWeapon(weapon[0]);
}

void SceneBase::Update()
{
	// �J����
	camera->Update();

	// �Q�[���p�I�u�W�F�N�g
	player->Update();
	for (int i = 0; i < 10; i++)
	{
		if (objCubeRed[i]->GetUsedFlag()) objCubeRed[i]->Update();
		if (objCubeGreen[i]->GetUsedFlag()) objCubeGreen[i]->Update();
		if (objCubeBlue[i]->GetUsedFlag()) objCubeBlue[i]->Update();
	}
	objCastle->Update();

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		enemy[i]->Update();
	}

	// obj�X�V
	objSkydome->Update();
	objGround->Update();
	objWall->Update();
	weapon[0]->Update();

	// fbx�X�V

	// �X�v���C�g
	spriteTitle->Update();
}

void SceneBase::SaveStage()
{

}

void SceneBase::LoadStage()
{
}
