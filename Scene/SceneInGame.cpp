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

		stgObjects[i]->Update();
	}

	player->SetPosition({
		objCastle->GetPosition().x,
		objCastle->GetPosition().y + 40,
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

	MeshCollider* col = static_cast<MeshCollider*>(objGroundGrid->ObjectOBJ::GetCollider());
	tri = col->GetTriangle();
	//Enemy::CreateNaviMesh(tri);
	//tri = Enemy::GetNaviMesh();
}

void SceneInGame::Update()
{
	UpdateMenu();

	switch (GameManager::GetFinishState())
	{
	case CONTINUE:
		if (!menuActive) break;

		if (buttonTitle->Click(MOUSE_LEFT))
		{
			for (int i = 0; i < stgObjects.size(); i++)
			{
				stgObjects[i]->ResetStatus();
			}
			SceneManager::SetScene(TITLE);
			windowActive = false;
			return;
		}

		if (buttonRetry->Click(MOUSE_LEFT))
		{
			GameManager::Restart();

			for (int i = 0; i < stgObjects.size(); i++)
			{
				if (stgObjects[i]->GetHP() > 0) stgObjects[i]->ResetStatus();
			}

			player->SetPosition({
				objCastle->GetPosition().x,
				objCastle->GetPosition().y + 40,
				objCastle->GetPosition().z
				});
			player->AnimationTimerReset();
			windowActive = false;
			return;
		}

		break;

	case WAVE_CLEARE:

		if (buttonTitle->Click(MOUSE_LEFT))
		{
			for (int i = 0; i < stgObjects.size(); i++)
			{
				stgObjects[i]->ResetStatus();
			}
			SceneManager::SetScene(TITLE);
			windowActive = false;
			return;
		}

		if (buttonNext->Click(MOUSE_LEFT))
		{
			GameManager::ChangeNextWave();

			player->SetPosition({
				objCastle->GetPosition().x,
				objCastle->GetPosition().y + 40,
				objCastle->GetPosition().z
				});
			player->AnimationTimerReset();
			windowActive = false;

			return;
		}

		break;

	case WAVE_FAILED:

		if (buttonTitle->Click(MOUSE_LEFT))
		{
			for (int i = 0; i < stgObjects.size(); i++)
			{
				stgObjects[i]->ResetStatus();
			}
			SceneManager::SetScene(TITLE);
			windowActive = false;
			return;
		}

		if (buttonRetry->Click(MOUSE_LEFT))
		{
			GameManager::Restart();

			for (int i = 0; i < stgObjects.size(); i++)
			{
				if (stgObjects[i]->GetHP() > 0) stgObjects[i]->ResetStatus();
			}

			player->SetPosition({
				objCastle->GetPosition().x,
				objCastle->GetPosition().y + 40,
				objCastle->GetPosition().z
				});
			player->AnimationTimerReset();
			windowActive = false;
			return;
		}

	default:
		break;
	}



	GameManager::SetStageObject(objCastle);

	if (menuActive) return;

	numbers[enemy_count]->SetValue(GameManager::GetEnemyDefeatCount(), 0, 64, {32,64});
	numbers[enemy_count_max]->SetValue(GameManager::GetEnemyCountMax(), 32 * 4, 64, {32,64});
	numbers[wait_timer]->SetValue(GameManager::GetWaitTimerSeconds(), (float)WinApp::window_width / 2 - 16, 128 + 200, {32, 64});
	numbers[wave_number]->SetValue(GameManager::GetWaveNumber(), (float)WinApp::window_width / 2 - 32, 128 + 64, {32, 64});
	numbers[wave_number]->SetSize({ 64,96 });

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

	for (int i = 0; i < 2; i++)
	{
		objGroundGridLine[i]->Draw();
	}

	for (int i = 0; i < 8; i++)
	{
		objWall[i]->Draw();
	}


	for (int i = 0; i < stgObjects.size(); i++)
	{
		stgObjects[i]->Draw();
	}

	player->ObjectOBJ::Draw();
	weapon[0]->ObjectOBJ::Draw();
	GameManager::Draw();

	ObjectOBJ::PostDraw();

	particle->Draw(dxCommon->GetCmdList());


	// FBXモデル
	ObjectFBX::PreDraw(dxCommon->GetCmdList());

	//player->ObjectFBX::Draw();

	ObjectFBX::PostDraw();

#pragma endregion

#pragma region 前景スプライト

	// スプライト描画前処理
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// スプライト描画
	meterPlayerHP->Draw();
	meterCastleHP->Draw();
	numbers[enemy_count]->Draw();
	numbers[enemy_count_max]->Draw();
	spriteFractionBar->Draw();

	DrawMenu();

	// テキスト描画
	text->DrawAll();

	spriteCommon->PostDraw();


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
	spriteWaveClear->SetColor({ 1.0f,1.0f,1.0f, alpha });
	spriteWaveFailed->SetColor({ 1.0f,1.0f,1.0f, alpha });
	spritePause->SetColor({ 1.0f,1.0f,1.0f, alpha });

	alpha = 1;
	buttonTitle->SetColor({ 1.0f,1.0f,1.0f, alpha });
	buttonNext->SetColor({ 1.0f,1.0f,1.0f, alpha });
	buttonRetry->SetColor({ 1.0f,1.0f,1.0f, alpha });
	spriteUIWindowBlue->SetColor({ 1.0f,1.0f,1.0f, alpha });
	spriteUIWindowBlue->SetSize({ windowSizeX, 256.0f });

	spriteWaveClear->Update();
	spriteWaveFailed->Update();
	spritePause->Update();

	buttonTitle->Update();
	buttonNext->Update();
	buttonRetry->Update();
	spriteUIWindowBlue->Update();
}

void SceneInGame::DrawMenu()
{
	if (GameManager::GetWaitTimer() > 0 && !menuActive)
	{
		numbers[wait_timer]->Draw();

		if (GameManager::IsFinalWave())
		{
			spriteWaveFinal->Draw();
		}
		else
		{
			spriteWave->Draw();
			numbers[wave_number]->Draw();
		}
	}

	if (menuActive)
	{
		spriteUIWindowBlue->Draw();

		if(windowActive)
		{
			switch (GameManager::GetFinishState())
			{
			case CONTINUE:
				buttonTitle->Draw();
				spritePause->Draw();
				buttonRetry->Draw();
				break;
			case WAVE_CLEARE:
				buttonTitle->Draw();
				spriteWaveClear->Draw();
				if (!GameManager::IsFinalWave()) buttonNext->Draw();
				break;
			case WAVE_FAILED:
				buttonTitle->Draw();
				spriteWaveFailed->Draw();
				buttonRetry->Draw();
				break;
			}
		}

		spriteCursor->Draw();
	}
}

void SceneInGame::RecreateNaviMesh()
{
	Enemy::CreateNaviMesh(tri);
}
