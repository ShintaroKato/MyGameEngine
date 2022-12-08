#pragma once
#include "StageObject.h"

class GameManager
{
public:
	// ゲームスタート
	static void Start();
	// ゲームリスタート
	static void Restart();
	// 更新
	static void Update();
	// 終了フラグの状態取得
	static int GetFinishState() { return finish; }
	// タイマー取得
	static int GetTimer() { return timer; }
	// タイマー取得(秒に換算)
	static int GetTimerSeconds() { return timer / 60; }
	// 待機時間取得
	static int GetWaitTimer() { return waitTimer; }
	// 待機時間取得(秒に換算)
	static int GetWaitTimerSeconds() { return waitTimer / 60; }
	// 守る対象のオブジェクトをセット
	static void SetStageObject(StageObject* stageObject) { GameManager::stageObject = stageObject; }

private:
	GameManager() = default;
	GameManager(const GameManager&) = delete;
	~GameManager() = default;
	GameManager& operator=(const GameManager&) = delete;
	// 開始前待機時間
	static const int waitTimerMax = 10 * 60;
	static int waitTimer;
	// 制限時間
	static const int timerMax = 60 * 60;
	static int timer;
	// スコア
	static int score;
	// ウェーブ数
	static const int waveMax = 1;
	static int wave;
	// ウェーブ終了
	static int finish; // 1でクリア判定 -1で失敗判定
	// 守る対象のオブジェクト
	static StageObject* stageObject;
};