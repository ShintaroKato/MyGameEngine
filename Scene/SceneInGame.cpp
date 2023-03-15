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
		if (stgObjects[i]->GetTag() == CASTLE_OBJECT) objCastle = stgObjects[i];
	}

	objSkydome->SetScale({ 5,5,5 });
	objGroundGrid->ObjectOBJ::SetScale({ 5,5,5 });

	player->SetPosition({
		objCastle->GetPosition().x,
		objCastle->GetPosition().y + 20,
		objCastle->GetPosition().z
		});
	player->Update();

	camera->SetTarget(player->GetPosition());
	camera->SetEye({ 0,2,5 });

	menuActivate = false;

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

	spriteWave->SetPosition({ WinApp::window_width / 2, 128 });
	spriteWaveClear->SetPosition({ WinApp::window_width / 2, 128 });
	spriteWaveFailed->SetPosition({ WinApp::window_width / 2, 128 });
	spriteWaveFinal->SetPosition({ WinApp::window_width / 2, 128 });
	spritePause->SetPosition({ WinApp::window_width / 2, 128 });

	spriteUIFrame->SetSize({ 512,256 });
	spriteUIFrame->SetAnchorPoint({ 0.5f,0.5f });
	spriteUIFrame->SetPosition({ WinApp::window_width / 2,WinApp::window_height / 2 });
	spriteUIFrame->Update();

	GameManager::Start();
	GameManager::SetEnemyModel(modelEnemy);

	SceneBase::Update();
}

void SceneInGame::Update()
{
	Menu();

	if ((menuActivate || GameManager::GetFinishState() != 0) &&
		buttonTitle->Click(MOUSE_LEFT))
	{
		for (int i = 0; i < stgObjects.size(); i++)
		{
			stgObjects[i]->ResetStatus();
		}
		SceneManager::SetScene(TITLE);

		return;
	}

	if (((menuActivate && GameManager::GetFinishState() == 0) || GameManager::GetFinishState() == -1) &&
		buttonRetry->Click(MOUSE_LEFT))
	{
		GameManager::Restart();
		UItimer = UItimerMax;
		UItimer--;

		player->SetPosition({
			objCastle->GetPosition().x,
			objCastle->GetPosition().y + 20,
			objCastle->GetPosition().z
			});

		return;
	}

	if (GameManager::GetFinishState() == 1 &&
		buttonNext->Click(MOUSE_LEFT))
	{
		GameManager::ChangeNextWave();
		UItimer = UItimerMax;
		UItimer--;

		player->SetPosition({
			objCastle->GetPosition().x,
			objCastle->GetPosition().y + 20,
			objCastle->GetPosition().z
			});

		return;
	}

	GameManager::SetStageObject(objCastle);

	camera->SetTarget({
		player->GetPosition().x,
		player->GetPosition().y + 2,
		player->GetPosition().z
		});

	if (UItimer > 0) return;

	numberTimer->SetSequence(GameManager::GetTimerSeconds(), 0, 64, { 32,64 });
	numberWaitTimer->SetSequence(GameManager::GetWaitTimerSeconds(), (float)WinApp::window_width / 2, 128 + 192, { 32, 64 });
	numberWave->SetSequence(GameManager::GetWaveNumber(), (float)WinApp::window_width / 2, 128 + 64, { 32, 64 });
	numberWave->SetSize({ 32,96 });

	meterPlayerHP->SetValue(player->GetHP(), player->GetHPMax());
	meterPlayerHP->Update();

	meterCastleHP->SetValue(objCastle->GetHP(), objCastle->GetHPMax());
	meterCastleHP->Update();

	spriteWave->Update();
	spriteWaveClear->Update();
	spriteWaveFailed->Update();
	spriteWaveFinal->Update();
	spritePause->Update();

	SceneBase::Update();
	for (int i = 0; i < stgObjects.size(); i++)
	{
		if (stgObjects[i]->GetUsedState() != UNUSED) stgObjects[i]->Update();
	}
	SortObjectCameraDistance();
	GameManager::Update();
	BulletManager::GetInstance()->Update();
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

	objWall->Draw();

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
	numberTimer->Draw();

	if (GameManager::GetWaitTimer() > 0 && !menuActivate)
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

	if(UItimer % 3 && UItimer > 0)
	{
		spriteUIFrame->Draw();

		if (menuActivate)
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

			spriteCursor->Draw();
		}
	}

	spriteCommon->PostDraw();

	// テキスト描画
	//text->DrawAll(dxCommon->GetCmdList());

#pragma endregion

#pragma endregion グラフィックスコマンド
}

void SceneInGame::Menu()
{
	if (!menuActivate)
	{
		// タイマーが少しでも進んでいたらカウント開始
		if (UItimer > 0) UItimer++;
		// タイマーが上限を超えたら停止してメニュー有効化
		if (UItimer > UItimerMax)
		{
			UItimer = UItimerMax;
			menuActivate = true;
		}

		// マウスカーソルの座標を画面中央に固定
		SetCursorPos(WinApp::window_width / 2, WinApp::window_height / 2);
		player->SetCameraMoveFlag(true);

		if (input->TriggerKey(DIK_ESCAPE) || GameManager::GetFinishState() != 0)
		{
			UItimer++;
		}
	}
	else
	{
		// タイマーが少しでも進んでいたらカウント開始
		if (UItimer < UItimerMax) UItimer--;
		// タイマーが上限を超えたら停止してメニュー無効化
		if (UItimer < 0)
		{
			UItimer = 0;
			menuActivate = false;
		}

		spriteCursor->SetPosition(input->GetMousePos2());
		spriteCursor->Update();
		player->SetCameraMoveFlag(false);

		if (input->TriggerKey(DIK_ESCAPE) && GameManager::GetFinishState() == 0)
		{
			UItimer--;
		}
	}

	buttonTitle->SetColor({ 1.0f,1.0f,1.0f, (float)(pow(UItimer,2) / pow(UItimerMax, 2)) });
	buttonNext->SetColor({ 1.0f,1.0f,1.0f, (float)UItimer / UItimerMax });
	buttonRetry->SetColor({ 1.0f,1.0f,1.0f, (float)UItimer / UItimerMax });
	spriteUIFrame->SetColor({ 1.0f,1.0f,1.0f, (float)UItimer / UItimerMax });
	spriteUIFrame->SetSize({ (512.0f / UItimerMax) * UItimer, 256.0f });

	buttonTitle->Update();
	buttonNext->Update();
	buttonRetry->Update();
	spriteUIFrame->Update();
}
