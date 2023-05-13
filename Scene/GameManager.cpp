#include "GameManager.h"
#include "ParticleManager.h"
#include "BulletManager.h"
#include <algorithm>

int GameManager::waitTimer = 0;
int GameManager::timerMax = 0;
int GameManager::timer = 0;
int GameManager::wave = 1;
int GameManager::scoreTotal = 0;
int GameManager::score = 0;
int GameManager::finish = 0;
int GameManager::level = 1;
int GameManager::spawnInterval = 0;
int GameManager::spawnIntervalMax = 20;
int GameManager::spawnCount = 0;
int GameManager::spawnCountMax = 60;
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
	wave = 2;

	Level();

	score = 0;
	enemyCountTotal = 0;
	groupCount = 0;
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

	DeleteEnemy();
	stageObject->ResetStatus();

	score = 0;
	enemyCountTotal = 0;
	groupCount = 0;
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

	case 0: // ゲーム継続

		// クリア判定の処理
		if (score >= enemyMaxAll * groupCountMax) finish = 1;

		if (waitTimer > 0) waitTimer--; // 待機時間
		//else if (timer > 0) timer--; // 制限時間
		//else finish = 1; // クリア判定

		// 失敗判定の処理
		if (stageObject != nullptr &&
			stageObject->GetHP() <= 0) finish = -1;

		if (waitTimer > 0) break;

		SpawnEnemyGroup();

		UpdateEnemy();

		break;

	case 1: // クリア
		break;

	case -1: // 失敗
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

void GameManager::Spawn()
{
	// 出現する敵のタイプをランダムで設定
	EnemyType enemy_type = (EnemyType)(rand() % EnemyType::TYPE_COUNT);

	// 決定された敵のタイプのカウントを１増やす
	enemyCount[enemy_type]++;

	if (enemyCount[enemy_type] > enemyCountMax[enemy_type])
	{
		// 余分に増えた分を減らす
		enemyCount[enemy_type]--;
		return;
	}

	if (!modelObjEnemy[enemy_type]) return;
	enemies.push_front(*Enemy::Create(modelObjEnemy[enemy_type]));
	Enemy& e = enemies.front();
	e.SetAllive(true);

	// 出現する方角を設定
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
	// 中心からの距離
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

	enemyCountTotal++;
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
		// 中心からの距離が一定値以上のとき
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
	// 敵が全滅していたら次の集団の出現タイマーカウント開始
	if (std::distance(enemies.begin(), enemies.end()) <= 0)
	{
		spawnCount++;

		if (spawnCount == spawnCountMax)
		{
			groupCount++;
		}

	}
	if (spawnCount > spawnCountMax)
	{
		spawnInterval++;

		//敵の数をカウント
		if (std::distance(enemies.begin(), enemies.end()) < enemyMaxAll)
		{
			if (spawnInterval >= spawnIntervalMax)
			{
				Spawn();
				spawnInterval = 0;
			}
		}
		else
		{
			spawnCount = 0;
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
	// 敵の出現上限初期化
	for (int i = 0; i < EnemyType::TYPE_COUNT; i++)
	{
		enemyCount[i] = 0;
		enemyCountMax[i] = 0;
	}
	enemyMaxAll = 0;

	switch (wave)
	{
	case 1:
		level = 1;
		break;
	case 2:
		level = 2;
		break;
	case 3:
		level = 3;
		break;
	case 4:
		level = 4;
		break;
	case 5:
		level = 5;
		break;
	}

	switch (level)
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

	// 敵の出現上限数の合計
	for (int i = 0; i < EnemyType::TYPE_COUNT; i++)
	{
		enemyMaxAll += enemyCountMax[i];
	}
}
