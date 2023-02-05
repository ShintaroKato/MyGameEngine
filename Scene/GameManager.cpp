#include "GameManager.h"
#include "ParticleManager.h"

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
	wave = 1;

	Level();

	waitTimer = waitTimerMax;
	timer = timerMax;
	score = 0;
	scoreTotal = 0;

	finish = 0;

	ParticleManager::GetInstance()->DeleteAllParticle();
}

void GameManager::Restart()
{
	Level();

	stageObject->ResetStatus();

	waitTimer = waitTimerMax;
	timer = timerMax;
	score = 0;

	finish = 0;

	ParticleManager::GetInstance()->DeleteAllParticle();
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

		break;

	case -1: // Ž¸”s

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
