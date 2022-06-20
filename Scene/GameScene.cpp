#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete fbxCube;
	delete fbxModelCube;
}

void GameScene::Initialize(DirectXCommon* dxCommon, SpriteCommon* sprCommon, Input* input, Audio* audio)
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

	//Object3d::SetCamera(camera);
	//Object3d::SetDevice(dxCommon->GetDev());

	// スプライト共通テクスチャ読み込み
	spriteCommon->LoadTexture(0, L"Resources/debugfont.png");
	spriteCommon->LoadTexture(1, L"Resources/background.png");

	// テキスト
	text = Text::GetInstance();
	text->Initialize(spriteCommon, 0);

	// スプライト
	spriteBG = Sprite::Create(spriteCommon, 1, { 0,0 }, {0,0});
	spriteBG->Update();

	//// obj.からモデルデータ読み込み
	//modelSphere = Model::LoadObj("sphere", true);
	//// 3Dオブジェクト生成
	//objSphere = Object3d::Create();
	//objSphere->SetCamera(camera);
	//// オブジェクトにモデルを紐づける
	//objSphere->SetModel(modelSphere);

	//objSphere->SetPosition({ 0,0,30 });
	//objSphere->Update();

	fbxModelCube = FBXLoader::GetInstance()->LoadModelFromFile("cube");
	fbxCube = ObjectFBX::Create();
	fbxCube->SetModel(fbxModelCube);
}

void GameScene::Update()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		SceneManager::SceneChangeTitle();
	}

	fbxCube->Update();

	spriteBG->Update();
}

void GameScene::Draw()
{
	dxCommon->PreDraw();

#pragma region 背景スプライト

	// スプライト描画前処理
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// スプライト描画
	spriteBG->Draw();

	spriteCommon->PostDraw();

#pragma endregion

#pragma region 3Dオブジェクト

	// 3Dオブジェクト描画前処理
	//Object3d::PreDraw(dxCommon->GetCmdList());

	//objSphere->Draw();

	//Object3d::PostDraw();

	fbxCube->Draw(dxCommon->GetCmdList());

#pragma endregion

#pragma region 前景スプライト

	// スプライト描画前処理
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// スプライト描画
	//spriteBG->Draw();

	// テキスト描画
	//text->DrawAll(dxCommon->GetCmdList());

	spriteCommon->PostDraw();

#pragma endregion


	dxCommon->PostDraw();
#pragma endregion グラフィックスコマンド
}
