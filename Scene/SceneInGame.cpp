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
<<<<<<< HEAD
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
=======
	SceneBase::Initialize(dxCommon, sprCommon, input, audio);
>>>>>>> CG4_Test

	// スプライト共通テクスチャ読み込み
	spriteCommon->LoadTexture(0, "debugfont.png");
	spriteCommon->LoadTexture(1, "background.png");

	// テキスト
	text = Text::GetInstance();
	text->Initialize(spriteCommon, 0);

	// スプライト
	spriteBG = Sprite::Create(spriteCommon, 1, { 0,0 }, {0,0});
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

	for (int i = 0; i < CUBE_RED_MAX; i++)
	{
		objCubeRed[i] = GameObject::Create(modelCubeRed);
		objCubeGreen[i] = GameObject::Create(modelCubeGreen);
		objCubeBlue[i] = GameObject::Create(modelCubeBlue);

		objCubeRed[i]->ObjectOBJ::SetScale({ 2,2,2 });
		objCubeGreen[i]->ObjectOBJ::SetScale({ 2,2,2 });
		objCubeBlue[i]->ObjectOBJ::SetScale({ 2,2,2 });

<<<<<<< HEAD
		objCubeRed[i]->ObjectOBJ::SetPosition({ 25,0,50 });
		objCubeGreen[i]->ObjectOBJ::SetPosition({ -25,0,50 });
		objCubeBlue[i]->ObjectOBJ::SetPosition({ 0,0,-25 });
	}

	objSkydome->SetScale({ 5,5,5 });
	objSkydome->Update();

	objGround->SetScale({ 5,5,5 });
	objGround->Update();

	player = Player::Create(fbxModelAnim);
	player->ObjectFBX::SetScale({ 2,2,2 });
	player->SetAnimationNumber(0);
	player->AnimationReset();
	player->Update();

	camera->SetTarget({0,0,0});
	camera->SetEye({ 0,50,-100 });
	camera->SetTarget(player->ObjectFBX::GetPosition());
=======
	fbxModelAnim = FBXLoader::GetInstance()->LoadModelFromFile("boneTest");
	fbxAnimTest = ObjectFBX::Create();
	fbxAnimTest->SetModel(fbxModelAnim);
	fbxAnimTest->SetPosition({ 0,-10,-80 });
	fbxAnimTest->SetRotation({ 0,0,0 });
	fbxAnimTest->SetAnimationNumber(0);
	fbxAnimTest->Update();

	camera->SetTarget({ 0,0,0 });
	camera->SetEye({ 0,0,-100 });
	camera->SetTarget(fbxAnimTest->GetPosition());
>>>>>>> CG4_Test
	camera->Update();
}

void SceneInGame::Update()
{
<<<<<<< HEAD
	camera->SetTarget({ 0,0,0 });

	camera->Update();

	objCubeRed[0]->Update();
	objCubeGreen[0]->Update();
	objCubeBlue[0]->Update();
=======
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

	camera->SetTarget(fbxAnimTest->GetPosition());

	camera->Update();

	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) ||
		input->PushKey(DIK_A) || input->PushKey(DIK_D))
	{
		XMFLOAT3 rot = fbxAnimTest->GetRotation();

		if (input->PushKey(DIK_W))
		{
			rot.x++;
		}
		else if (input->PushKey(DIK_S))
		{
			rot.x--;
		}
		else if (input->PushKey(DIK_D))
		{
			rot.y++;
		}
		else if (input->PushKey(DIK_A))
		{
			rot.y--;
		}

		fbxAnimTest->SetRotation(rot);
	}
>>>>>>> CG4_Test

	if (input->TriggerKey(DIK_SPACE))
	{
		
	}
<<<<<<< HEAD
=======
	if (input->PushKey(DIK_Y))
	{
		fbxAnimTest->AnimationPlay();
	}
	if (input->PushKey(DIK_T))
	{
		fbxAnimTest->AnimationStop();
	}
	if (input->PushKey(DIK_R))
	{
		fbxAnimTest->AnimationReset();
	}
>>>>>>> CG4_Test

	// obj更新
	objSkydome->Update();
	objGround->Update();
	// fbx更新
	player->Update();

	spriteBG->Update();
}

void SceneInGame::Draw()
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
