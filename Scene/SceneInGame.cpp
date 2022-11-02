#include "SceneInGame.h"

SceneInGame::SceneInGame()
{
}

SceneInGame::~SceneInGame()
{
	delete gManager;
	delete player;
}

void SceneInGame::Initialize(DirectXCommon* dxCommon, SpriteCommon* sprCommon, Input* input, Audio* audio)
{
	buttonTitle->SetPosition({ 0,0 });
	buttonTitle->SetSize({ 128,64 });

	for (int i = 0; i < CUBE_RED_MAX; i++)
	{
		objCubeRed[i]->PositionFix();
		objCubeGreen[i]->PositionFix();
		objCubeBlue[i]->PositionFix();

		objCubeRed[i]->SetUsedFlag(tmpFlag[i]);
		objCubeGreen[i]->SetUsedFlag(tmpFlag[i + 10]);
		objCubeBlue[i]->SetUsedFlag(tmpFlag[i + 20]);

		objCubeRed[i]->SetPosition(tmp[i]);
		objCubeGreen[i]->SetPosition(tmp[i + 10]);
		objCubeBlue[i]->SetPosition(tmp[i + 20]);
	}
	objCastle->PositionFix();
	objCastle->SetUsedFlag(true);
	objCastle->SetPosition(tmp[99]);

	objSkydome->SetScale({ 5,5,5 });
	objGroundGrid->ObjectOBJ::SetScale({ 5,5,5 });

	player->SetPosition({ 0, 0, 0 });
	player->Update();

	camera->SetEye({ 0,2,5 });
	camera->SetTarget(player->GetPosition());

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		enemy[i]->SetTargetPos(objCastle->GetPosition());
		enemy[i]->SetInGame(true);
		enemy[i]->SetAllive(false);
	}

	gManager = new GameManager();

	SceneBase::Update();
}

void SceneInGame::Update()
{
	cursorON = false;

	if (input->PushKey(DIK_LALT) || input->PushKey(DIK_RALT))
	{
		cursorON = true;
	}

	if(!cursorON)
	{
		// マウスカーソルの座標を画面中央に固定
		SetCursorPos(WinApp::window_width / 2, WinApp::window_height / 2);
		player->SetCameraMoveFlag(true);
	}
	else
	{
		spriteCursor->SetPosition(input->GetMousePos2());
		spriteCursor->Update();
		player->SetCameraMoveFlag(false);
	}

	if (input->TriggerKey(DIK_ESCAPE) || buttonTitle->Click(MOUSE_LEFT))
	{
		SceneManager::SetScene(TITLE);

		for (int i = 0; i < CUBE_RED_MAX; i++)
		{
			tmp[i] = objCubeRed[i]->ObjectOBJ::GetPosition();
			tmp[i + 10] = objCubeGreen[i]->ObjectOBJ::GetPosition();
			tmp[i + 20] = objCubeBlue[i]->ObjectOBJ::GetPosition();
		}

		tmp[99] = objCastle->ObjectOBJ::GetPosition();
	}

	camera->SetTarget({
		player->GetPosition().x,
		player->GetPosition().y + 2,
		player->GetPosition().z
		});

	gManager->Update();

	numberTimer->SetSequence(gManager->GetTimerSeconds(), 0, 64, { 32,64 });

	SceneBase::Update();
}

void SceneInGame::Draw()
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

	// OBJモデル
	ObjectOBJ::PreDraw(dxCommon->GetCmdList());

	//objSphere->Draw();
	objSkydome->Draw();
	objGroundGrid->ObjectOBJ::Draw();

	for (int i = 0; i < 10; i++)
	{
		if (objCubeRed[i]->GetUsedFlag()) objCubeRed[i]->ObjectOBJ::Draw();
		if (objCubeGreen[i]->GetUsedFlag()) objCubeGreen[i]->ObjectOBJ::Draw();
		if (objCubeBlue[i]->GetUsedFlag()) objCubeBlue[i]->ObjectOBJ::Draw();
	}

	if(objCastle->GetUsedFlag()) objCastle->ObjectOBJ::Draw();

	objWall->Draw();

	player->ObjectOBJ::Draw();
	weapon[0]->ObjectOBJ::Draw();

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		enemy[i]->ObjectOBJ::Draw();
	}

	ObjectOBJ::PostDraw();

	// FBXモデル
	//player->ObjectFBX::Draw(dxCommon->GetCmdList());

#pragma endregion

#pragma region 前景スプライト

	// スプライト描画前処理
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	buttonTitle->Draw();

	// スプライト描画
	//spriteBG->Draw();
	if (cursorON) spriteCursor->Draw();
	numberTimer->Draw();

	spriteCommon->PostDraw();

	// テキスト描画
	//text->DrawAll(dxCommon->GetCmdList());

#pragma endregion

#pragma endregion グラフィックスコマンド
}
