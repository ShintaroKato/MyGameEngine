#include "GameManager.h"

int GameManager::waitTimer = 0;
int GameManager::timerMax = 0;
int GameManager::timer = 0;
int GameManager::wave = 1;
int GameManager::scoreTotal = 0;
int GameManager::score = 0;
int GameManager::finish = 0;
int GameManager::level = 1;
int GameManager::enemyMax = 1;
StageObject* GameManager::stageObject = nullptr;

void GameManager::Start()
{
	Level();

	wave = 1;
	waitTimer = waitTimerMax;
	timer = timerMax;
	score = 0;
	scoreTotal = 0;

	finish = 0;
}

void GameManager::Restart()
{
	Level();

	stageObject->ResetStatus();

	waitTimer = waitTimerMax;
	timer = timerMax;
	score = 0;

	finish = 0;
}

void GameManager::Update()
{
	switch (finish)
	{
	case 0: // ゲーム継続

		// クリア判定の処理
		if (waitTimer > 0) waitTimer--; // 待機時間
		else if (timer > 0) timer--; // 制限時間
		else finish = 1; // クリア判定

		// 失敗判定の処理
		if (stageObject != nullptr &&
			stageObject->GetHP() <= 0) finish = -1;

		break;

	case 1: // クリア

		break;

	case -1: // 失敗

		break;
	}
}

void GameManager::ChangeNextWave()
{
	if (wave == waveMax) return;

	wave++;
	scoreTotal += score;

	Restart();
}

void GameManager::Level()
{
	switch (wave)
	{
	case 1:
		level = 1;
		break;
	case 2:
	case 3:
		level = 2;
		break;
	case 4:
	case 5:
		level = 3;
		break;
	}

	switch (level)
	{
	case 1:
		enemyMax = 2;
		timerMax = 60 * 30;
		break;
	case 2:
		enemyMax = 4;
		timerMax = 60 * 50;
		break;
	case 3:
		enemyMax = 8;
		timerMax = 60 * 60;
		break;
	}
}
