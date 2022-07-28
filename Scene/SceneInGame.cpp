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

	ObjectOBJ::SetDevice(dxCommon->GetDev());

	ObjectFBX::SetDevice(dxCommon->GetDev());
	SceneBase::Initialize(dxCommon, sprCommon, input, audio);


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
	modelPlayer = ModelOBJ::LoadObj("player");
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
	// obj更新
	objSkydome->Update();
	objGround->Update();
	// fbx更新

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
	player->ObjectOBJ::Draw();
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

void SceneInGame::LoadDat()
{

}
