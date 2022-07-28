#include "SceneInGame.h"
#include "PostEffect.h"

SceneInGame::SceneInGame()
{
}

SceneInGame::~SceneInGame()
{
	delete player;
}

void SceneInGame::Initialize(DirectXCommon* dxCommon, SpriteCommon* sprCommon, Input* input, Audio* audio)
{
	// nullptr�`�F�b�N
	assert(dxCommon);
	assert(sprCommon);
	assert(input);

	this->dxCommon = dxCommon;
	this->spriteCommon = sprCommon;
	this->input = input;

	// �J��������
	camera = new Camera();
	camera->Initialize(WinApp::window_width, WinApp::window_height);

	ObjectOBJ::SetDevice(dxCommon->GetDev());

	ObjectFBX::SetDevice(dxCommon->GetDev());
	SceneBase::Initialize(dxCommon, sprCommon, input, audio);


	// �X�v���C�g���ʃe�N�X�`���ǂݍ���
	spriteCommon->LoadTexture(0, "debugfont.png");
	spriteCommon->LoadTexture(1, "background.png");

	// �e�L�X�g
	text = Text::GetInstance();
	text->Initialize(spriteCommon, 0);

	// �X�v���C�g
	spriteBG = Sprite::Create(spriteCommon, 1, { 0,0 }, {0,0});
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
	objGround = ObjectOBJ::Create();
	// �I�u�W�F�N�g�Ƀ��f����R�Â���
	objSkydome->SetModelOBJ(modelSkydome);
	objGround->SetModelOBJ(modelGround);

	for (int i = 0; i < CUBE_RED_MAX; i++)
	{
		objCubeRed[i] = GameObject::Create(modelCubeRed);
		objCubeGreen[i] = GameObject::Create(modelCubeGreen);
		objCubeBlue[i] = GameObject::Create(modelCubeBlue);

		objCubeRed[i]->ObjectOBJ::SetPosition(SceneBase::tmp[i]);
		objCubeGreen[i]->ObjectOBJ::SetPosition(SceneBase::tmp[i + 10]);
		objCubeBlue[i]->ObjectOBJ::SetPosition(SceneBase::tmp[i + 20]);
		
		objCubeRed[i]->ChangeFixed();
		objCubeGreen[i]->ChangeFixed();
		objCubeBlue[i]->ChangeFixed();
	}

	objSkydome->SetScale({ 5,5,5 });
	objSkydome->Update();

	objGround->SetScale({ 5,5,5 });
	objGround->Update();

	player = Player::Create(modelPlayer);
	player->ObjectOBJ::SetCamera(camera);
	player->SetPosition({ 0, 0, 0 });
	player->Update();

	camera->SetTarget({0,0,0});
	camera->SetEye({ 0,5,-10 });
	camera->SetTarget(player->GetPosition());
	camera->Update();
}

void SceneInGame::Update()
{
	camera->SetTarget(player->GetPosition());

	camera->Update();

	if (input->TriggerKey(DIK_ESCAPE))
	{
		SceneManager::SetScene(TITLE);
	}
	// obj�X�V
	objSkydome->Update();
	objGround->Update();
	// fbx�X�V

	player->Update();

	for (int i = 0; i < 10; i++)
	{
		objCubeRed[i]->Update();
		objCubeGreen[i]->Update();
		objCubeBlue[i]->Update();
	}

	spriteBG->Update();
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

	//objSphere->Draw();
	objSkydome->Draw();
	player->ObjectOBJ::Draw();
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

	spriteCommon->PostDraw();

	// �e�L�X�g�`��
	//text->DrawAll(dxCommon->GetCmdList());

#pragma endregion

#pragma endregion �O���t�B�b�N�X�R�}���h
}

void SceneInGame::LoadDat()
{

}
