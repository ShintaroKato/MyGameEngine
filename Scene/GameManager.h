#pragma once
#include "Enemy.h"
#include "StageObject.h"

enum SpawnDirection
{
	North,
	East,
	South,
	West
};

enum GameState
{
	CONTINUE,
	WAVE_FAILED,
	WAVE_CLEARE
};

class GameManager
{
public:
	// ゲームスタート
	static void Start();
	// ゲームリスタート
	static void Restart();
	// 更新
	static void Update();
	// 描画
	static void Draw();
	// エネミーを削除
	static void DeleteEnemy();
	// 終了フラグの状態取得
	static int GetFinishState() { return finish; }
	// タイマー取得
	static int GetTimer() { return timer; }
	// タイマー取得(秒に換算)
	static int GetTimerSeconds() { return timer / 60; }
	// 今までに倒した敵の合計を取得
	static int GetEnemyDefeatCount() { return score; }
	// 敵の数の最大値を取得
	static int GetEnemyCountMax() { return groupCountMax * enemyMaxAll; }
	// 待機時間取得
	static int GetWaitTimer() { return waitTimer; }
	// 待機時間取得(秒に換算)
	static int GetWaitTimerSeconds() { return waitTimer / 60; }
	// 守る対象のオブジェクトをセット
	static void SetStageObject(StageObject* stageObject) { GameManager::stageObject = stageObject; }
	// 最後のウェーブか否かを判定
	static bool IsFinalWave() { return wave == waveMax; }
	// ウェーブの番号を取得
	static int GetWaveNumber() { return wave; }
	// 次のウェーブに切り替え
	static void ChangeNextWave();
	// エネミーのモデルを保存(OBJ)
	static void SetEnemyModel(ModelOBJ* model[])
	{
		modelObjEnemy.clear();

		for (int i = 0; i < EnemyType::TYPE_COUNT; i++)
		{
			modelObjEnemy.push_back(model[i]);
		}
	}
	// エネミーのモデルを保存(FBX)
	static void SetEnemyModel(ModelFBX* model[])
	{
		modelFbxEnemy.clear();

		for (int i = 0; i < EnemyType::TYPE_COUNT; i++)
		{
			modelFbxEnemy.push_back(model[i]);
		}
	}

private:
	// レベル
	static void Level();
	// エネミー出現
	static bool Spawn();
	// エネミーを一定数の集団として出現させる
	static void SpawnEnemyGroup();
	// エネミーを更新
	static void UpdateEnemy();
private:
	// 敵オブジェクト
	static std::forward_list<Enemy> enemies;
	static std::vector<ModelOBJ*> modelObjEnemy;
	static std::vector<ModelFBX*> modelFbxEnemy;
	static int spawn_dir;
	static float spawn_angle;
	// 守る対象のオブジェクト
	static StageObject* stageObject;
	// 敵の一つのグループにおけるタイプごとの出現上限数
	static int enemyCountMax[EnemyType::TYPE_COUNT];
	// 敵の一つのグループにおける出現上限数の合計
	static int enemyMaxAll;
	// 敵をタイプごとにカウント
	static int enemyCount[EnemyType::TYPE_COUNT];
	// 敵が今までに出現した数
	static int enemyCountTotal;
	// 敵の集団一つ分が消えてから再出現する間隔
	static int groupRespawnTimer;
	static int groupRespawnTimerMax;
	// 敵の集団の数
	static int groupCount;
	static int groupCountMax;
	// 一体が出現する間隔
	static int enemySpawnTimer;
	static int enemySpawnTimerMax;
	static XMFLOAT3 spawnPos;
	// 開始前待機時間
	static const int waitTimerMax = 5 * 60;
	static int waitTimer;
	// 制限時間
	static int timerMax;
	static int timer;
	// スコア
	static int scoreTotal;
	static int score;
	// ウェーブ数
	static const int waveMax = 5;
	static int wave;
	// ウェーブ終了
	static int finish; // 1でクリア判定 -1で失敗判定

private:
	GameManager() = default;
	GameManager(const GameManager&) = delete;
	~GameManager() = default;
	GameManager& operator=(const GameManager&) = delete;
};