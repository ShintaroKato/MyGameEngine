#include "SceneInGame.h"
#include "SceneManager.h"
#include "BulletManager.h"

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

	for (int i = 0; i < stgObjects.size(); i++)
	{
		stgObjects[i]->SetInGameFlag(true);
		stgObjects[i]->ResetStatus();
		if (stgObjects[i]->GetTag() == STAGE_OBJECT_CASTLE) objCastle = stgObjects[i];
	}

	player->SetPosition({
		objCastle->GetPosition().x,
		objCastle->GetPosition().y + 20,
		objCastle->GetPosition().z
		});
	player->Update();

	camera->SetCameraDistance(10);
	camera->SetTarget(player->GetPosition());
	camera->SetEye({ 0,2,5 });

	menuActive = false;

	buttonTitle->SetPosition({ WinApp::window_width / 2, 400 });
	buttonTitle->SetAnchorPoint({ 0.5f,0.5f });
	buttonTitle->SetSize({ 128,64 });

	buttonNext->SetPosition({ WinApp::window_width / 2, 300 });
	buttonNext->SetAnchorPoint({ 0.5f,0.5f });
	buttonNext->SetSize({ 128,64 });

	buttonRetry->SetPosition({ WinApp::window_width / 2, 300 });
	buttonRetry->SetAnchorPoint({ 0.5f,0.5f });
	buttonRetry->SetSize({ 128,64 });

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
	meterCastleHP->SetValue(objCastle->GetHP(), objCastle->GetHPMax());

	spriteFractionBar->SetPosition({ 32 * 3, 64 });
	spriteWave->SetPosition({ WinApp::window_width / 2, 128 });
	spriteWaveClear->SetPosition({ WinApp::window_width / 2, 128 });
	spriteWaveFailed->SetPosition({ WinApp::window_width / 2, 128 });
	spriteWaveFinal->SetPosition({ WinApp::window_width / 2, 128 });
	spritePause->SetPosition({ WinApp::window_width / 2, 128 });

	spriteUIWindowBlue->SetSize({ 0,256 });
	spriteUIWindowBlue->SetAnchorPoint({ 0.5f,0.5f });
	spriteUIWindowBlue->SetPosition({ WinApp::window_width / 2,WinApp::window_height / 2 });
	spriteUIWindowBlue->Update();

	GameManager::Start();
	GameManager::SetEnemyModel(modelEnemy);

	camera->SetInGameFlag(true);

	SceneBase::Update();
}

void SceneInGame::Update()
{
	UpdateMenu();

	if ((menuActive || GameManager::GetFinishState() != 0) &&
		buttonTitle->Click(MOUSE_LEFT))
	{
		for (int i = 0; i < stgObjects.size(); i++)
		{
			stgObjects[i]->ResetStatus();
		}
		SceneManager::SetScene(TITLE);

		return;
	}

	if (((menuActive && GameManager::GetFinishState() == 0) || GameManager::GetFinishState() == -1) &&
		buttonRetry->Click(MOUSE_LEFT))
	{
		GameManager::Restart();

		player->SetPosition({
			objCastle->GetPosition().x,
			objCastle->GetPosition().y + 20,
			objCastle->GetPosition().z
			});

		windowActive = false;
		return;
	}

	if (GameManager::GetFinishState() == 1 &&
		buttonNext->Click(MOUSE_LEFT))
	{
		GameManager::ChangeNextWave();

		player->SetPosition({
			objCastle->GetPosition().x,
			objCastle->GetPosition().y + 20,
			objCastle->GetPosition().z
			});

		windowActive = false;

		return;
	}

	GameManager::SetStageObject(objCastle);

	if (menuActive) return;

	numberEnemyCount->SetSequence(GameManager::GetEnemyCountTotal(), 0, 64, { 32,64 });
	numberEnemyCountMax->SetSequence(GameManager::GetEnemyCountMax(),32 * 4, 64, { 32,64 });
	numberWaitTimer->SetSequence(GameManager::GetWaitTimerSeconds(), (float)WinApp::window_width / 2, 128 + 192, { 32, 64 });
	numberWave->SetSequence(GameManager::GetWaveNumber(), (float)WinApp::window_width / 2, 128 + 64, { 32, 64 });
	numberWave->SetSize({ 32,96 });

	meterPlayerHP->SetValue(player->GetHP(), player->GetHPMax());
	meterPlayerHP->Update();

	meterCastleHP->SetValue(objCastle->GetHP(), objCastle->GetHPMax());
	meterCastleHP->Update();

	spriteFractionBar->Update();
	spriteWave->Update();
	spriteWaveClear->Update();
	spriteWaveFailed->Update();
	spriteWaveFinal->Update();
	spritePause->Update();

	for (int i = 0; i < stgObjects.size(); i++)
	{
		if (stgObjects[i]->GetUsedState() != UNUSED) stgObjects[i]->Update();
	}
	SortObjectCameraDistance();
	GameManager::Update();
	BulletManager::GetInstance()->Update();
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

	objSkydome->Draw();
	objSkydomeSpace->Draw();
	objGroundGrid->ObjectOBJ::Draw();

	for (int i = 0; i < 8; i++)
	{
		objWall[i]->Draw();
	}

	player->ObjectOBJ::Draw();
	player->ObjectOBJ::Draw();
	weapon[0]->ObjectOBJ::Draw();
	GameManager::Draw();

	for (int i = 0; i < stgObjects.size(); i++)
	{
		stgObjects[i]->Draw();
	}

	ObjectOBJ::PostDraw();

	particle->Draw(dxCommon->GetCmdList());


	// FBXモデル
	//player->ObjectFBX::Draw(dxCommon->GetCmdList());

#pragma endregion

#pragma region 前景スプライト

	// スプライト描画前処理
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// スプライト描画
	meterPlayerHP->Draw();
	meterCastleHP->Draw();
	numberEnemyCount->Draw();
	numberEnemyCountMax->Draw();
	spriteFractionBar->Draw();

	DrawMenu();

	spriteCommon->PostDraw();

	// テキスト描画
	//text->DrawAll(dxCommon->GetCmdList());

#pragma endregion

#pragma endregion グラフィックスコマンド
}

void SceneInGame::UpdateMenu()
{
	if (!menuActive)
	{
		// マウスカーソルの座標を画面中央に固定
		SetCursorPos(WinApp::window_width / 2, WinApp::window_height / 2);
		camera->SetCameraMoveFlag(true);

		if (input->TriggerKey(DIK_ESCAPE) || GameManager::GetFinishState() != 0)
		{
			windowActive = true;
		}
	}
	else
	{
		spriteCursor->SetPosition(input->GetMousePos2());
		spriteCursor->Update();
		camera->SetCameraMoveFlag(false);

		if (input->TriggerKey(DIK_ESCAPE) && GameManager::GetFinishState() == 0)
		{
			windowActive = false;
		}
	}

	int count = 10;
	float max = 512.0f;
	float windowSizeX = spriteUIWindowBlue->GetSize().x;

	if (windowActive)
	{
		if (windowSizeX < max)	windowSizeX += max / count;
		menuActive = true;
	}
	else
	{
		if (windowSizeX > 0)	windowSizeX -= max / count;
		else					menuActive = false;
	}

	float alpha = windowSizeX / max;
	if ((int)(alpha * 100) % 3 == 0)
	{
		alpha = 0;
	}

	buttonTitle->SetColor({ 1.0f,1.0f,1.0f, alpha });
	buttonNext->SetColor({ 1.0f,1.0f,1.0f, alpha });
	buttonRetry->SetColor({ 1.0f,1.0f,1.0f, alpha });
	spriteUIWindowBlue->SetColor({ 1.0f,1.0f,1.0f, alpha });
	spriteUIWindowBlue->SetSize({ windowSizeX, 256.0f });

	buttonTitle->Update();
	buttonNext->Update();
	buttonRetry->Update();
	spriteUIWindowBlue->Update();
}

void SceneInGame::DrawMenu()
{
	if (GameManager::GetWaitTimer() > 0 && !menuActive)
	{
		numberWaitTimer->Draw();

		if (GameManager::IsFinalWave())
		{
			spriteWaveFinal->Draw();
		}
		else
		{
			spriteWave->Draw();
			numberWave->Draw();
		}
	}

	if (menuActive)
	{
		spriteUIWindowBlue->Draw();

		if(windowActive)
		{
			switch (GameManager::GetFinishState())
			{
			case 0:
				buttonTitle->Draw();
				spritePause->Draw();
				buttonRetry->Draw();
				break;
			case 1:
				buttonTitle->Draw();
				spriteWaveClear->Draw();
				if (!GameManager::IsFinalWave()) buttonNext->Draw();
				break;
			case -1:
				buttonTitle->Draw();
				spriteWaveFailed->Draw();
				buttonRetry->Draw();
				break;
			}
		}

		spriteCursor->Draw();
	}
}
