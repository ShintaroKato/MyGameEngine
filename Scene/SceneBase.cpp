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
	spriteCommon->LoadTexture(0, "debugfont.png");
	spriteCommon->LoadTexture(1, "title_transparent.png");

	// �e�L�X�g
	text = Text::GetInstance();
	text->Initialize(spriteCommon, 0);

	// �X�v���C�g
	spriteTitle = Sprite::Create(spriteCommon, 1, { 0,0 }, { 0,0 });
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

	player = Player::Create(modelPlayer);
	player->ObjectOBJ::SetCamera(camera);
}

void SceneBase::Update()
{
	// �J����
	camera->Update();

	// �Q�[���p�I�u�W�F�N�g
	player->Update();
	for (int i = 0; i < 10; i++)
	{
		objCubeRed[i]->Update();
		objCubeGreen[i]->Update();
		objCubeBlue[i]->Update();
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
