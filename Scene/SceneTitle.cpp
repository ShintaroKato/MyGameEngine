#include "SceneTitle.h"

SceneTitle::SceneTitle()
{
}

void SceneTitle::Initialize(DirectXCommon* dxCommon, SpriteCommon* sprCommon, Input* input, Audio* audio)
{
	// nullptrチェック
	assert(dxCommon);
	assert(sprCommon);
	assert(input);

	this->dxCommon = dxCommon;
	this->spriteCommon = sprCommon;
	this->input = input;

	// スプライト共通テクスチャ読み込み
	spriteCommon->LoadTexture(0, "debugfont.png");
	spriteCommon->LoadTexture(1, "background.png");

	// テキスト
	text->GetInstance()->Initialize(spriteCommon, 0);
	// スプライト
	spriteBG = Sprite::Create(spriteCommon, 1, { 0,0 }, { 0,0 });
	spriteBG->Update();

	//// obj.からモデルデータ読み込み
	//modelSphere = Model::LoadObj("sphere");
	//// 3Dオブジェクト生成
	//objSphere = Object3d::Create();
	//// オブジェクトにモデルを紐づける
	//objSphere->SetModel(modelSphere);

	//objSphere->SetPosition({ 0,0,30 });
	//objSphere->Update();
}

void SceneTitle::Update()
{
	if (input->PushKey(DIK_SPACE))
	{
		SceneManager::SceneChange();
	}

	spriteBG->Update();
}

void SceneTitle::Draw()
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

#pragma endregion

#pragma region 前景スプライト

	// スプライト描画前処理
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// スプライト描画
	spriteBG->Draw();

	// テキスト描画
	//text->DrawAll(dxCommon->GetCmdList());

	spriteCommon->PostDraw();

#pragma endregion


	dxCommon->PostDraw();
#pragma endregion グラフィックスコマンド
}
