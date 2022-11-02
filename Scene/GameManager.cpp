#include "GameManager.h"

GameManager::GameManager()
{
}

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
	case 0: // �Q�[���p��
		if (timer > 0) timer--;
		else finish = 1;
		break;

	case 1: // �N���A
		
		if (wave < waveMax) wave++;
		break;

	case -1: // ���s

		break;
	}
}
