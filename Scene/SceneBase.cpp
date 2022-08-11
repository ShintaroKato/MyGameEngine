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
	spriteCommon->LoadTexture(1, "background.png");

	// �e�L�X�g
	text = Text::GetInstance();
	text->Initialize(spriteCommon, 0);

	// �X�v���C�g
	spriteBG = Sprite::Create(spriteCommon, 1, { 0,0 }, { 0,0 });
	spriteBG->Update();

	// .obj���烂�f���f�[�^�ǂݍ���
	modelSkydome = ModelOBJ::LoadObj("skydome", true);
	modelGround = ModelOBJ::LoadObj("ground", true);
	modelPlayer = ModelOBJ::LoadObj("player");
	modelCubeRed = ModelOBJ::LoadObj("cube64Red", true);
	modelCubeGreen = ModelOBJ::LoadObj("cube64Green", true);
	modelCubeBlue = ModelOBJ::LoadObj("cube64Blue", true);

	// .fbx���烂�f���f�[�^�ǂݍ���
	fbxModelAnim = FBXLoader::GetInstance()->LoadModelFromFile("boneTest");

	// 3D�I�u�W�F�N�g����
	objSkydome = ObjectOBJ::Create();
	// �I�u�W�F�N�g�Ƀ��f����R�Â���
	objSkydome->SetModelOBJ(modelSkydome);

	objGround = TouchableObject::Create(modelGround);

	for (int i = 0; i < CUBE_RED_MAX; i++)
	{
		objCubeRed[i] = GameObject::Create(modelCubeRed);
		objCubeGreen[i] = GameObject::Create(modelCubeGreen);
		objCubeBlue[i] = GameObject::Create(modelCubeBlue);
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

	// obj�X�V
	objSkydome->Update();
	objGround->ObjectOBJ::Update();

	// fbx�X�V

	// �X�v���C�g
	spriteBG->Update();
}
