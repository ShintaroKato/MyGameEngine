#include "GameManager.h"
#include "ParticleEmitter.h"
#include "BulletManager.h"
#include <algorithm>

int GameManager::waitTimer = 0;
int GameManager::timerMax = 0;
int GameManager::timer = 0;
int GameManager::wave = 1;
int GameManager::scoreTotal = 0;
int GameManager::score = 0;
int GameManager::finish = 0;
int GameManager::enemySpawnTimer = 0;
int GameManager::enemySpawnTimerMax = 20;
int GameManager::groupRespawnTimer = 0;
int GameManager::groupRespawnTimerMax = 60;
int GameManager::groupCount = 0;
int GameManager::groupCountMax = 5;
int GameManager::enemyCountMax[EnemyType::TYPE_COUNT] = {};
int GameManager::enemyMaxAll = 0;
int GameManager::enemyCount[EnemyType::TYPE_COUNT] = {};
int GameManager::enemyCountTotal = 0;
int GameManager::spawn_dir = 0;
float GameManager::spawn_angle = 0;
XMFLOAT3 GameManager::spawnPos{};
StageObject* GameManager::stageObject = nullptr;
std::forward_list<Enemy> GameManager::enemies;
std::vector<ModelOBJ*> GameManager::modelObjEnemy;
std::vector<ModelFBX*> GameManager::modelFbxEnemy;

void GameManager::Start()
{
	wave = 1;

	Level();

	enemyCountTotal = 0;
	groupCount = 0;
	waitTimer = waitTimerMax;
	timer = timerMax;
	score = 0;
	scoreTotal = 0;

	finish = CONTINUE;

	ParticleManager::GetInstance()->DeleteAllParticle();
}

void GameManager::Restart()
{
	Level();

	DeleteEnemy();
	stageObject->ResetStatus();

	enemyCountTotal = 0;
	groupCount = 0;
	waitTimer = waitTimerMax;
	timer = timerMax;
	score = 0;

	finish = CONTINUE;

	ParticleManager::GetInstance()->DeleteAllParticle();
}

void GameManager::Update()
{

	switch (finish)
	{

	case CONTINUE: // �Q�[���p��

		// �N���A����̏���
		if (score >= enemyMaxAll * groupCountMax) finish = WAVE_CLEARE;

		if (waitTimer > 0) waitTimer--; // �ҋ@����
		//else if (timer > 0) timer--; // ��������
		//else finish = 1; // �N���A����

		// ���s����̏���
		if (stageObject != nullptr &&
			stageObject->GetHP() <= 0) finish = WAVE_FAILED;

		if (waitTimer > 0) break;

		SpawnEnemyGroup();

		UpdateEnemy();

		break;

	case WAVE_FAILED: // ���s
		break;

	case WAVE_CLEARE: // �N���A
		break;
	}
}

void GameManager::Draw()
{
	if (GameManager::GetWaitTimer() > 0) return;
	if (GameManager::GetFinishState() != 0) return;

	for (std::forward_list<Enemy>::iterator it = enemies.begin(); it != enemies.end(); it++)
	{
		it->Draw();
	}
}

bool GameManager::Spawn()
{
	// �o������G�̃^�C�v�������_���Őݒ�
	EnemyType enemy_type = (EnemyType)(rand() % EnemyType::TYPE_COUNT);

	// ���肳�ꂽ�G�̃^�C�v�̃J�E���g���P���₷
	enemyCount[enemy_type]++;

	if (enemyCount[enemy_type] > enemyCountMax[enemy_type])
	{
		// �]���ɑ������������炷
		enemyCount[enemy_type]--;
		return false;
	}

	if (!modelObjEnemy[enemy_type]) return false;
	enemies.push_front(*Enemy::Create(modelObjEnemy[enemy_type]));
	Enemy& e = enemies.front();
	e.SetAllive(true);

	// �o��������p��ݒ�
	spawn_dir = rand() % 4;
	switch (spawn_dir)
	{
	case North:
		spawn_angle = 0;
		break;
	case East:
		spawn_angle = 90;
		break;
	case South:
		spawn_angle = 180;
		break;
	case West:
		spawn_angle = 270;
		break;
	default:
		break;
	}
	// ���S����̋���
	float distance = 80;
	float rand_range = 30;
	XMFLOAT3 random = {
		(float)rand() / RAND_MAX * rand_range - rand_range / 2,
		0,
		(float)rand() / RAND_MAX * rand_range - rand_range / 2
	};
	spawnPos.x = distance * sin(XMConvertToRadians(spawn_angle)) + random.x;
	spawnPos.z = distance * cos(XMConvertToRadians(spawn_angle)) + random.z;
	e.SetPosition(spawnPos);
	e.SetType(enemy_type);
	e.SetTargetPos(stageObject);
	e.SetSpawnAnimationActiveFlag(true);

	enemyCountTotal++;

	return true;
}

void GameManager::DeleteEnemy()
{
	for (std::forward_list<Enemy>::iterator it = enemies.begin(); it != enemies.end(); it++)
	{
		CollisionManager::GetInstance()->RemoveCollider(it->GetCollider());
	}
	enemies.clear();
}

void GameManager::UpdateEnemy()
{
	for (std::forward_list<Enemy>::iterator it = enemies.begin(); it != enemies.end(); it++)
	{
		// ���S����̋��������l�ȏ�̂Ƃ�
		if (sqrtf(
			(it->GetPosition().x * it->GetPosition().x) +
			(it->GetPosition().z * it->GetPosition().z)) >= 60)
		{
			it->SetTargetPos({ 0,0,0 });
		}

		it->Update();

		if (it->GetDefeatFlag()) score++;
		if (!it->GetAliveFlag())
		{
			enemyCount[it->GetType()]--;
			CollisionManager::GetInstance()->RemoveCollider(it->GetCollider());
		}
	}
	enemies.remove_if([](Enemy& x) { return x.GetAliveFlag() == false; });
}

void GameManager::SpawnEnemyGroup()
{
	// �G���S�ł��Ă����玟�̃O���[�v�̏o���^�C�}�[�J�E���g�J�n
	if (groupRespawnTimer < groupRespawnTimerMax)
	{
		groupRespawnTimer++;
	}
	else if (groupRespawnTimer == groupRespawnTimerMax)
	{
		// ���ɗ��܂��Ă���J�E���g��������
		for (int i = 0; i < EnemyType::TYPE_COUNT; i++)
		{
			enemyCount[i] = 0;
		}
		enemyCountTotal = 0;

		// ���܂ŏo�������O���[�v�̃J�E���g�𑝂₷
		groupCount++;

		groupRespawnTimer++;
	}

	if (groupRespawnTimer > groupRespawnTimerMax)
	{
		//�G�̐����J�E���g
		if (enemyCountTotal < enemyMaxAll)
		{
			// 1�̏o������܂ł̊Ԋu���J�E���g
			enemySpawnTimer++;

			// �ő�łȂ���Α��₷
			if (enemySpawnTimer >= enemySpawnTimerMax)
			{
				if(Spawn())
				{
					enemySpawnTimer = 0;
				}
			}
		}
		else if (std::distance(enemies.begin(), enemies.end()) <= 0)
		{
			// �G�̐����ő傩�S�ł��Ă�����O���[�v�o���p�^�C�}�[�����Z�b�g
			groupRespawnTimer = 0;
		}
	}
}

void GameManager::ChangeNextWave()
{
	if (wave == waveMax) return;

	wave++;
	scoreTotal += score;
	DeleteEnemy();
	for (int i = 0; i < EnemyType::TYPE_COUNT; i++)
	{
		enemyCount[i] = 0;
	}
	BulletManager::GetInstance()->Delete();
	ParticleManager::GetInstance()->DeleteAllParticle();
	Restart();
}

void GameManager::Level()
{
	// �G�̏o�����������
	for (int i = 0; i < EnemyType::TYPE_COUNT; i++)
	{
		enemyCount[i] = 0;
		enemyCountMax[i] = 0;
	}
	enemyMaxAll = 0;

	switch (wave)
	{
	case 1:
		enemyCountMax[STRAIGHT] = 10;
		groupCountMax = 1;
		//timerMax = 60 * 30;
		break;
	case 2:
		enemyCountMax[STRAIGHT] = 5;
		enemyCountMax[FLYING] = 3;
		groupCountMax = 2;
		//timerMax = 60 * 30;
		break;
	case 3:
		enemyCountMax[STRAIGHT] = 10;
		enemyCountMax[ROUTE_SEARCH] = 5;
		enemyCountMax[FLYING] = 2;
		groupCountMax = 2;
		//timerMax = 60 * 40;
		break;
	case 4:
		enemyCountMax[ROUTE_SEARCH] = 20;
		enemyCountMax[FLYING] = 5;
		groupCountMax = 2;
		//timerMax = 60 * 40;
		break;
	case 5:
		enemyCountMax[STRAIGHT] = 10;
		enemyCountMax[ROUTE_SEARCH] = 20;
		enemyCountMax[FLYING] = 5;
		groupCountMax = 3;
		//timerMax = 60 * 50;
		break;
	}

	// �G�̈�̃O���[�v�ɂ�����o��������̍��v
	for (int i = 0; i < EnemyType::TYPE_COUNT; i++)
	{
		enemyMaxAll += enemyCountMax[i]; 
	}
}
