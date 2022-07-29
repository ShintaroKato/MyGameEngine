#include "SceneTitle.h"

SceneTitle::SceneTitle()
{
}

void SceneTitle::Initialize(DirectXCommon* dxCommon, SpriteCommon* sprCommon, Input* input, Audio* audio)
{
	SceneBase::Initialize(dxCommon, sprCommon, input, audio);

	SceneBase::Update();
}

void SceneTitle::Update()
{
	if (input->TriggerKey(DIK_1))
	{
		SceneManager::SetScene(EDIT);
	}
	if (input->TriggerKey(DIK_2))
	{
		SceneManager::SetScene(GAME);
	}

	SceneBase::Update();
}

void SceneTitle::Draw()
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
	//ObjectOBJ::PreDraw(dxCommon->GetCmdList());

	//objSphere->Draw();

	//ObjectOBJ::PostDraw();

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

#pragma endregion グラフィックスコマンド
}
