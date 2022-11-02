#include "SceneTitle.h"

SceneTitle::SceneTitle()
{
}

void SceneTitle::Initialize(DirectXCommon* dxCommon, SpriteCommon* sprCommon, Input* input, Audio* audio)
{
	for (int i = 0; i < CUBE_RED_MAX; i++)
	{
		objCubeRed[i]->PositionFix();
		objCubeGreen[i]->PositionFix();
		objCubeBlue[i]->PositionFix();

		objCubeRed[i]->SetPosition(tmp[i]);
		objCubeGreen[i]->SetPosition(tmp[i + 10]);
		objCubeBlue[i]->SetPosition(tmp[i + 20]);

		objCubeRed[i]->SetUsedFlag(tmpFlag[i]);
		objCubeGreen[i]->SetUsedFlag(tmpFlag[i + 10]);
		objCubeBlue[i]->SetUsedFlag(tmpFlag[i + 20]);
	}
	objCastle->PositionFix();
	objCastle->SetPosition(tmp[99]);
	objCastle->SetUsedFlag(tmpFlag[99]);

	objSkydome->SetScale({ 5,5,5 });
	objGroundGrid->ObjectOBJ::SetScale({ 5,5,5 });

	player->SetAllive(false);
	player->SetInGameFlag(false);
	player->SetCameraDistance(80);

	camera->SetTarget(player->GetPosition());
	camera->SetEye({ 0,30,-100 });

	buttonStart->SetPosition({ WinApp::window_width / 2,WinApp::window_height - 256 });
	buttonStart->SetSize({ 128,64 });
	buttonStart->SetAnchorPoint({ 0.5f,0.5f });
	
	buttonEdit->SetPosition({ WinApp::window_width / 2,WinApp::window_height - 256 + 64 + 20 });
	buttonEdit->SetSize({ 128,64 });
	buttonEdit->SetAnchorPoint({ 0.5f,0.5f });

	SceneBase::Update();
}

void SceneTitle::Update()
{
	spriteCursor->SetPosition(input->GetMousePos2());

	if (input->TriggerKey(DIK_1) || buttonEdit->Click(MOUSE_LEFT))
	{
		SceneManager::SetScene(EDIT);
	}
	if (input->TriggerKey(DIK_2) || buttonStart->Click(MOUSE_LEFT))
	{
		SceneManager::SetScene(GAME);
	}

	if (input->PushMouse(MOUSE_RIGHT))
	{
		player->SetCameraMoveFlag(true);
	}
	else
	{
		player->SetCameraMoveFlag(false);
	}

	buttonEdit->Update();
	buttonStart->Update();

	spriteCursor->Update();


	SceneBase::Update();
}

void SceneTitle::Draw()
{
#pragma region 背景スプライト

	// スプライト描画前処理
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// スプライト描画
	spriteTitle->Draw();

	spriteCommon->PostDraw();

#pragma endregion

#pragma region 3Dオブジェクト

	// 3Dオブジェクト描画前処理
	// OBJモデル
	ObjectOBJ::PreDraw(dxCommon->GetCmdList());

	objSkydome->Draw();
	objGroundGrid->ObjectOBJ::Draw();

	for (int i = 0; i < 10; i++)
	{
		if (objCubeRed[i]->GetUsedFlag()) objCubeRed[i]->ObjectOBJ::Draw();
		if (objCubeGreen[i]->GetUsedFlag()) objCubeGreen[i]->ObjectOBJ::Draw();
		if (objCubeBlue[i]->GetUsedFlag()) objCubeBlue[i]->ObjectOBJ::Draw();
	}

	objCastle->ObjectOBJ::Draw();

	objWall->Draw();

	ObjectOBJ::PostDraw();

	// FBXモデル
	//player->ObjectFBX::Draw(dxCommon->GetCmdList());

#pragma endregion

#pragma region 前景スプライト

	// スプライト描画前処理
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	buttonEdit->Draw();
	buttonStart->Draw();

	// スプライト描画
	spriteTitle->Draw();
	spriteCursor->Draw();
	// テキスト描画
	//text->DrawAll(dxCommon->GetCmdList());

	spriteCommon->PostDraw();

#pragma endregion

#pragma endregion グラフィックスコマンド
}
