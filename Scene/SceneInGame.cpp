#include "SceneInGame.h"
#include "SceneManager.h"

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
		SceneBase::LoadStage(objCubeRed[i], true);
		SceneBase::LoadStage(objCubeGreen[i], true);
		SceneBase::LoadStage(objCubeBlue[i], true);
	}
	SceneBase::LoadStage(objCastle, true);

	objSkydome->SetScale({ 5,5,5 });
	objGroundGrid->ObjectOBJ::SetScale({ 5,5,5 });

	player->SetPosition({
		objCastle->GetPosition().x,
		objCastle->GetPosition().y + 10,
		objCastle->GetPosition().z
		});
	player->Update();

	camera->SetTarget(player->GetPosition());
	camera->SetEye({ 0,2,5 });

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		enemy[i]->SetTargetPos(objCastle);
		enemy[i]->SetInGame(true);
		enemy[i]->SetAllive(false);
	}

	menuON = false;

	buttonTitle->SetPosition({ WinApp::window_width / 2, 300 });
	buttonTitle->SetAnchorPoint({ 0.5f,0.5f });
	buttonTitle->SetSize({ 128,64 });

	meterPlayerHP->SetPosition({ 0, WinApp::window_height - 64 });
	meterPlayerHP->SetSize(
		{ 320, 64 },
		{ 320, 64 },
		{ 320, 64 });
	meterPlayerHP->SetValue(player->GetHP(), player->GetHPMax());

	meterCastleHP->SetPosition({ 0, WinApp::window_height - 128 });
	meterCastleHP->SetSize(
		{ 320, 64 },
		{ 320, 64 },
		{ 320, 64 });
	meterCastleHP->SetValue(player->GetHP(), player->GetHPMax());

	spriteWaveClear->SetPosition({ WinApp::window_width / 2, 128 });
	spriteWaveFailed->SetPosition({ WinApp::window_width / 2, 128 });
	spritePause->SetPosition({ WinApp::window_width / 2, 128 });

	GameManager::Start();

	SceneBase::Update();
}

void SceneInGame::Update()
{
	Menu();

	if ((menuON || GameManager::GetFinishState() != 0) && buttonTitle->Click(MOUSE_LEFT))
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

	GameManager::SetGameObject(objCastle);

	camera->SetTarget({
		player->GetPosition().x,
		player->GetPosition().y + 2,
		player->GetPosition().z
		});

	if (menuON) return;

	numberTimer->SetSequence(GameManager::GetTimerSeconds(), 0, 64, { 32,64 });
	numberWaitTimer->SetSequence(GameManager::GetWaitTimerSeconds(), WinApp::window_width / 2, 128, { 32, 64 });

	buttonTitle->Update();

	meterPlayerHP->SetValue(player->GetHP(), player->GetHPMax());
	meterPlayerHP->Update();

	meterCastleHP->SetValue(objCastle->GetHP(), objCastle->GetHPMax());
	meterCastleHP->Update();

	spriteWaveClear->Update();
	spriteWaveFailed->Update();
	spritePause->Update();

	GameManager::Update();

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
		if (objCubeRed[i]->GetUsedFlag()) objCubeRed[i]->Draw();
		if (objCubeGreen[i]->GetUsedFlag()) objCubeGreen[i]->Draw();
		if (objCubeBlue[i]->GetUsedFlag()) objCubeBlue[i]->Draw();
	}

	if(objCastle->GetUsedFlag()) objCastle->Draw();

	objWall->Draw();

	player->ObjectOBJ::Draw();
	weapon[0]->ObjectOBJ::Draw();

	if(GameManager::GetFinishState() == 0)
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

	// スプライト描画
	meterPlayerHP->Draw();
	meterCastleHP->Draw();
	numberTimer->Draw();

	if(GameManager::GetFinishState() == 0 && menuON)
	{
		spritePause->Draw();
	}
	else if (GameManager::GetWaitTimer() > 0)
	{
		numberWaitTimer->Draw();
	}

	if (GameManager::GetFinishState() == 1)
	{
		spriteWaveClear->Draw();
	}
	if (GameManager::GetFinishState() == -1)
	{
		spriteWaveFailed->Draw();
	}

	if (GameManager::GetFinishState() != 0 || menuON)
	{
		buttonTitle->Draw();
		spriteCursor->Draw();
	}

	//spriteBG->Draw();

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

		if (input->TriggerKey(DIK_ESCAPE) || GameManager::GetFinishState() != 0)
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
