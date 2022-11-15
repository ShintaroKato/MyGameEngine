#include "GameManager.h"

int GameManager::timer = 0;
int GameManager::wave = 0;
int GameManager::score = 0;
int GameManager::finish = 0;

void GameManager::Start()
{
	timer = timerMax;
	score = 0;

	wave = 1;
	finish = 0;
}

void GameManager::Update()
{
	switch (finish)
	{
	case 0: // ƒQ[ƒ€Œp‘±
		if (timer > 0) timer--;
		else finish = 1;
		break;

	case 1: // ƒNƒŠƒA
		
		if (wave < waveMax) wave++;
		break;

	case -1: // ¸”s

		break;
	}
}
