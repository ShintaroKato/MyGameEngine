#include "SceneInGame.h"

SceneInGame::SceneInGame()
{
}

SceneInGame::~SceneInGame()
{
	delete fbxCube;
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

	//ObjectOBJ::SetCamera(camera);
	ObjectOBJ::SetDevice(dxCommon->GetDev());

	ObjectFBX::SetCamera(camera);
	ObjectFBX::SetDevice(dxCommon->GetDev());

	// �X�v���C�g���ʃe�N�X�`���ǂݍ���
	spriteCommon->LoadTexture(0, "debugfont.png");
	spriteCommon->LoadTexture(1, "background.png");

	// �e�L�X�g
	text = Text::GetInstance();
	text->Initialize(spriteCommon, 0);

	// �X�v���C�g
	spriteBG = Sprite::Create(spriteCommon, 1, { 0,0 }, {0,0});
	spriteBG->Update();

	// obj.���烂�f���f�[�^�ǂݍ���
	modelSphere = ModelOBJ::LoadObj("sphere", true);
	// 3D�I�u�W�F�N�g����
	objSphere = ObjectOBJ::Create();
	// �I�u�W�F�N�g�Ƀ��f����R�Â���
	objSphere->SetModelOBJ(modelSphere);

	objSphere->SetPosition({ 0,0,10});
	objSphere->SetCamera(camera);
	objSphere->Update();

	fbxModelCube = FBXLoader::GetInstance()->LoadModelFromFile("boneTest");
	fbxCube = ObjectFBX::Create();
	fbxCube->SetModel(fbxModelCube);
	fbxCube->SetPosition({ 0,-10,10 });
	fbxCube->SetRotation({ 0,0,0 });

	fbxCube->Update();

	camera->SetTarget({0,0,0});
	camera->SetEye({ 0,0,-100 });
	camera->SetTarget(fbxCube->GetPosition());
	camera->Update();
}

void SceneInGame::Update()
{
	if (input->PushKey(DIK_UP))
	{
		camera->CameraMoveVector({ 0,1,0 });
	}
	else if (input->PushKey(DIK_DOWN))
	{
		camera->CameraMoveVector({ 0,-1,0 });
	}
	else if (input->PushKey(DIK_RIGHT))
	{
		camera->CameraMoveVector({ 1,0,0 });
	}
	else if (input->PushKey(DIK_LEFT))
	{
		camera->CameraMoveVector({ -1,0,0 });
	}
	else
	{
		camera->CameraMoveVector({ 0,0,0 });
	}

	camera->SetTarget(objSphere->GetPosition());

	camera->Update();

	if (input->TriggerKey(DIK_SPACE))
	{
		SceneManager::SetScene(TITLE);
	}

	fbxCube->Update();
	objSphere->Update();

	spriteBG->Update();
}

void SceneInGame::Draw()
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
	ObjectOBJ::PreDraw(dxCommon->GetCmdList());

	//objSphere->Draw();

	ObjectOBJ::PostDraw();

	fbxCube->Draw(dxCommon->GetCmdList());

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


	dxCommon->PostDraw();
#pragma endregion �O���t�B�b�N�X�R�}���h
}
