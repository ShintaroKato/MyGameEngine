#include "PostEffectScene.h"

PostEffectScene::PostEffectScene()
{
}

PostEffectScene::~PostEffectScene()
{
}

void PostEffectScene::Initialize(DirectXCommon* dxCommon, SpriteCommon* spriteCommon, Input* input, Audio* audio)
{
	SceneBase::Initialize(dxCommon, spriteCommon, input, audio);
	// カメラ生成
	camera = new Camera();
	camera->Initialize(WinApp::window_width, WinApp::window_height);

	//ObjectOBJ::SetCamera(camera);
	ObjectOBJ::SetDevice(dxCommon->GetDev());

	ObjectFBX::SetCamera(camera);
	ObjectFBX::SetDevice(dxCommon->GetDev());

	// スプライト共通テクスチャ読み込み
	spriteCommon->LoadTexture(0, "debugfont.png");
	spriteCommon->LoadTexture(100, "loading.png");

	// スプライト
	spriteBG = Sprite::Create(spriteCommon, 100, { 0,0 }, { 0,0 });
	spriteBG->Update();

	camera->SetTarget({ 0,0,0 });
	camera->SetEye({ 0,0,-90 });
	camera->Update();
}

void PostEffectScene::Update()
{
	camera->Update();

	spriteBG->Update();
}

void PostEffectScene::Draw()
{
#pragma region 背景スプライト

	// スプライト描画前処理
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// スプライト描画
	//spriteBG->Draw();

	spriteCommon->PostDraw();

#pragma endregion

#pragma region 3Dオブジェクト

	// 3Dオブジェクト描画前処理
	ObjectOBJ::PreDraw(dxCommon->GetCmdList());

	ObjectOBJ::PostDraw();

#pragma endregion

#pragma region 前景スプライト

	// スプライト描画前処理
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// スプライト描画
	spriteBG->Draw();

	spriteCommon->PostDraw();

	// テキスト描画
	//text->DrawAll(dxCommon->GetCmdList());

#pragma endregion

#pragma endregion グラフィックスコマンド
}
