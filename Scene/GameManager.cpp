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
	case 0: // ƒQ[ƒ€Œp‘±

		// ƒNƒŠƒA”»’è‚Ìˆ—
		if (waitTimer > 0) waitTimer--; // ‘Ò‹@ŽžŠÔ
		else if (timer > 0) timer--; // §ŒÀŽžŠÔ
		else finish = 1; // ƒNƒŠƒA”»’è

		// Ž¸”s”»’è‚Ìˆ—
		if (stageObject != nullptr &&
			stageObject->GetHP() <= 0) finish = -1;

		break;

	case 1: // ƒNƒŠƒA
		
		if (wave < waveMax) wave++;
		break;

	case -1: // Ž¸”s

		break;
	}
}
