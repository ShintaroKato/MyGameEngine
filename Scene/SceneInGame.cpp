#include "SceneInGame.h"

SceneInGame::SceneInGame()
{
}

SceneInGame::~SceneInGame()
{
	delete player;
}

void SceneInGame::Initialize(DirectXCommon* dxCommon, SpriteCommon* sprCommon, Input* input, Audio* audio)
{
	SceneBase::Initialize(dxCommon, sprCommon, input, audio);

	for (int i = 0; i < CUBE_RED_MAX; i++)
	{
		objCubeRed[i]->ObjectOBJ::SetPosition(SceneBase::tmp[i]);
		objCubeGreen[i]->ObjectOBJ::SetPosition(SceneBase::tmp[i + 10]);
		objCubeBlue[i]->ObjectOBJ::SetPosition(SceneBase::tmp[i + 20]);

		objCubeRed[i]->ChangeFixed();
		objCubeGreen[i]->ChangeFixed();
		objCubeBlue[i]->ChangeFixed();
	}

	objSkydome->SetScale({ 5,5,5 });
	objGround->SetScale({ 5,5,5 });

	player->SetPosition({ 0, 0, 0 });
	player->Update();

	camera->SetEye({ 0,5,-10 });
	camera->SetTarget(player->GetPosition());

	SceneBase::Update();
}

void SceneInGame::Update()
{
	if (input->TriggerKey(DIK_ESCAPE))
	{
		SceneManager::SetScene(TITLE);
	}

	camera->SetTarget(player->GetPosition());

	SceneBase::Update();
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
