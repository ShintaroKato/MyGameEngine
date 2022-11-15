#include "GameManager.h"

int GameManager::waitTimer = 0;
int GameManager::timer = 0;
int GameManager::wave = 0;
int GameManager::score = 0;
int GameManager::finish = 0;

void GameManager::Start()
{
	waitTimer = waitTimerMax;
	timer = timerMax;
	score = 0;

	wave = 1;
	finish = 0;
}

void GameManager::Update()
{
	switch (finish)
	{
	case 0: // ゲーム継続
		if (waitTimer > 0) waitTimer--; // 待機時間
		else if (timer > 0) timer--; // 制限時間
		else finish = 1; // クリア判定
		break;

	case 1: // クリア
		
		if (wave < waveMax) wave++;
		break;

	case -1: // 失敗

		break;
	}
}
