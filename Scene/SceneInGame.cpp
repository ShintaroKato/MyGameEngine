#include "SceneInGame.h"
#include "SceneManager.h"

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
	SceneBase::Initialize(dxCommon, sprCommon, input, audio);

	for (int i = 0; i < CUBE_RED_MAX; i++)
	{
		SceneBase::LoadStage(objCubeRed[i]);
		SceneBase::LoadStage(objCubeGreen[i]);
		SceneBase::LoadStage(objCubeBlue[i]);
	}
	SceneBase::LoadStage(objCastle);

	objSkydome->SetScale({ 5,5,5 });
	objGroundGrid->ObjectOBJ::SetScale({ 5,5,5 });

	player->SetPosition({ 0, 0, 0 });
	player->Update();

	camera->SetTarget(player->GetPosition());
	camera->SetEye({ 0,2,5 });

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		enemy[i]->SetTargetPos(objCastle->GetPosition());
		enemy[i]->SetInGame(true);
		enemy[i]->SetAllive(false);
	}

	menuON = false;

	buttonTitle->SetPosition({ 0, WinApp::window_height - 128 });
	buttonTitle->SetSize({ 128,64 });

	gManager->Start();

	SceneBase::Update();
}

void SceneInGame::Update()
{
	Menu();

	if (menuON && buttonTitle->Click(MOUSE_LEFT))
	{
		for (int i = 0; i < CUBE_RED_MAX; i++)
		{
			SceneBase::SaveStage(objCubeRed[i]);
			SceneBase::SaveStage(objCubeGreen[i]);
			SceneBase::SaveStage(objCubeBlue[i]);
		}
		SceneBase::SaveStage(objCastle);

		SceneManager::SetScene(TITLE);

		return;
	}

	camera->SetTarget({
		player->GetPosition().x,
		player->GetPosition().y + 2,
		player->GetPosition().z
		});

	numberTimer->SetSequence(gManager->GetTimerSeconds(), 0, 32, { 32,64 });

	gManager->Update();

	buttonTitle->Update();

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

	if(gManager->GetFinishState() == 0)
	{
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			enemy[i]->ObjectOBJ::Draw();
		}
	}

	ObjectOBJ::PostDraw();

	// FBXモデル
	//player->ObjectFBX::Draw(dxCommon->GetCmdList());

#pragma endregion

#pragma region 前景スプライト

	// スプライト描画前処理
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	if(menuON)
	{
		buttonTitle->Draw();
	}

	// スプライト描画
	//spriteBG->Draw();
	if (menuON) spriteCursor->Draw();
	numberTimer->Draw();

	spriteCommon->PostDraw();

	// テキスト描画
	//text->DrawAll(dxCommon->GetCmdList());

#pragma endregion

#pragma endregion グラフィックスコマンド
}

void SceneInGame::Menu()
{
	if (!menuON)
	{
		// マウスカーソルの座標を画面中央に固定
		SetCursorPos(WinApp::window_width / 2, WinApp::window_height / 2);
		player->SetCameraMoveFlag(true);

		if (input->TriggerKey(DIK_ESCAPE))
		{
			menuON = true;
		}
	}
	else
	{
		spriteCursor->SetPosition(input->GetMousePos2());
		spriteCursor->Update();
		player->SetCameraMoveFlag(false);

		if (input->TriggerKey(DIK_ESCAPE))
		{
			menuON = false;
		}
	}
}
