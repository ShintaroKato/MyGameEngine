#include "GameManager.h"

int GameManager::waitTimer = 0;
int GameManager::timer = 0;
int GameManager::wave = 0;
int GameManager::score = 0;
int GameManager::finish = 0;
StageObject* GameManager::stageObject = nullptr;

void GameManager::Start()
{
	waitTimer = waitTimerMax;
	timer = timerMax;
	score = 0;

	wave = 1;
	finish = 0;
}

void GameManager::Restart()
{
	waitTimer = waitTimerMax;
	timer = timerMax;

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
		
		if (wave < waveMax) wave++;
		break;

	case -1: // 失敗

		break;
	}
}
