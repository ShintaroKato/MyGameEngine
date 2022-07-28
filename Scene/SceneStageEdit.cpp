#include "SceneStageEdit.h"
#include "PostEffect.h"

SceneStageEdit::SceneStageEdit()
{
}

SceneStageEdit::~SceneStageEdit()
{
	delete player;
}

void SceneStageEdit::Initialize(DirectXCommon* dxCommon, SpriteCommon* sprCommon, Input* input, Audio* audio)
{
	// nullptrチェック
	assert(dxCommon);
	assert(sprCommon);
	assert(input);

	this->dxCommon = dxCommon;
	this->spriteCommon = sprCommon;
	this->input = input;

	// カメラ生成
	camera = new Camera();
	camera->Initialize(WinApp::window_width, WinApp::window_height);

	ObjectOBJ::SetCamera(camera);
	ObjectOBJ::SetDevice(dxCommon->GetDev());

	ObjectFBX::SetCamera(camera);
	ObjectFBX::SetDevice(dxCommon->GetDev());
	SceneBase::Initialize(dxCommon, sprCommon, input, audio);


	// スプライト共通テクスチャ読み込み
	spriteCommon->LoadTexture(0, "debugfont.png");
	spriteCommon->LoadTexture(1, "background.png");

	// テキスト
	text = Text::GetInstance();
	text->Initialize(spriteCommon, 0);

	// スプライト
	spriteBG = Sprite::Create(spriteCommon, 1, { 0,0 }, { 0,0 });
	spriteBG->Update();

	// .objからモデルデータ読み込み
	modelSkydome = ModelOBJ::LoadObj("skydome", true);
	modelGround = ModelOBJ::LoadObj("ground", true);
	modelCubeRed = ModelOBJ::LoadObj("cube64Red", true);
	modelCubeGreen = ModelOBJ::LoadObj("cube64Green", true);
	modelCubeBlue = ModelOBJ::LoadObj("cube64Blue", true);

	// .fbxからモデルデータ読み込み
	fbxModelAnim = FBXLoader::GetInstance()->LoadModelFromFile("boneTest");

	// 3Dオブジェクト生成
	objSkydome = ObjectOBJ::Create();
	objGround = ObjectOBJ::Create();
	// オブジェクトにモデルを紐づける
	objSkydome->SetModelOBJ(modelSkydome);
	objGround->SetModelOBJ(modelGround);

	for (int i = 0; i < OBJECT_MAX / 3; i++)
	{
		objCubeRed[i] = GameObject::Create(modelCubeRed);
		objCubeGreen[i] = GameObject::Create(modelCubeGreen);
		objCubeBlue[i] = GameObject::Create(modelCubeBlue);
	}
	objCubeRed[0]->ObjectOBJ::SetPosition({ 25,0,50 });
	objCubeGreen[0]->ObjectOBJ::SetPosition({ -25,0,50 });
	objCubeBlue[0]->ObjectOBJ::SetPosition({ 0,0,-25 });

	objCubeRed[1]->ObjectOBJ::SetPosition({ 50,0,25 });
	objCubeGreen[1]->ObjectOBJ::SetPosition({ -50,0,25 });
	objCubeBlue[1]->ObjectOBJ::SetPosition({ 0,0,-50 });


	objSkydome->SetScale({ 5,5,5 });
	objSkydome->Update();

	objGround->SetScale({ 5,5,5 });
	objGround->Update();

	player = Player::Create(fbxModelAnim);
	player->ObjectFBX::SetScale({ 2,2,2 });
	player->SetAnimationNumber(0);
	player->AnimationReset();
	player->Update();

	camera->SetTarget({ 0,0,0 });
	camera->SetEye({ 0,50,-100 });
	camera->SetTarget(player->ObjectFBX::GetPosition());
	camera->Update();
}

void SceneStageEdit::Update()
{
	camera->SetTarget({ 0,0,0 });

	camera->Update();


	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) ||
		input->PushKey(DIK_LEFT) || input->PushKey(DIK_RIGHT))
	{
		if (input->PushKey(DIK_UP))
		{
			camera->CameraMoveVector({ 0,1,0 });
		}
		else if (input->PushKey(DIK_DOWN))
		{
			camera->CameraMoveVector({ 0,-1,0 });
		}
		if (input->PushKey(DIK_LEFT))
		{
			camera->CameraMoveVector({ 1,0,0 });
		}
		else if (input->PushKey(DIK_RIGHT))
		{
			camera->CameraMoveVector({ -1,0,0 });
		}
	}
	else
	{
		camera->CameraMoveVector({ 0,0,0 });
	}

	camera->SetTarget(player->GetPosition());

	camera->Update();

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
	if (input->TriggerKey(DIK_R))
	{
		objCubeRed[0]->ObjectOBJ::SetPosition({ 25,0,50 });
		objCubeGreen[0]->ObjectOBJ::SetPosition({ -25,0,50 });
		objCubeBlue[0]->ObjectOBJ::SetPosition({ 0,0,-25 });

		objCubeRed[1]->ObjectOBJ::SetPosition({ 50,0,25 });
		objCubeGreen[1]->ObjectOBJ::SetPosition({ -50,0,25 });
		objCubeBlue[1]->ObjectOBJ::SetPosition({ 0,0,-50 });
	}

	// obj更新
	objSkydome->Update();
	objGround->Update();
	// fbx更新
	//player->Update();

	for (int i = 0; i < 10; i++)
	{
		objCubeRed[i]->Update();
		objCubeGreen[i]->Update();
		objCubeBlue[i]->Update();
	}

	spriteBG->Update();
}

void SceneStageEdit::Draw()
{
#pragma region 背景スプライト

	// スプライト描画前処理
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// スプライト描画
	spriteBG->Draw();

	spriteCommon->PostDraw();

#pragma endregion

#pragma region 3Dオブジェクト

	// 3Dオブジェクト描画前処理

	// OBJモデル
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

	// FBXモデル
	//player->ObjectFBX::Draw(dxCommon->GetCmdList());

#pragma endregion

#pragma region 前景スプライト

	// スプライト描画前処理
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// スプライト描画
	//spriteBG->Draw();

	spriteCommon->PostDraw();

	// テキスト描画
	//text->DrawAll(dxCommon->GetCmdList());

#pragma endregion

#pragma endregion グラフィックスコマンド
}

void SceneStageEdit::SaveDat()
{
	//FILE* file = fopen("Resources/Data/stageData.dat", "wb");
	//assert(!file);

	//fwrite()
}
