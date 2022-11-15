#pragma once

class GameManager
{
public:
	static void Start();
	static void Update();
	// 終了フラグの状態取得
	static int GetFinishState() { return finish; }
	// タイマー取得
	static int GetTimer() { return timer; }
	// タイマー取得(秒に換算)
	static int GetTimerSeconds() { return timer / 60; }

private:
	GameManager() = default;
	GameManager(const GameManager&) = delete;
	~GameManager() = default;
	GameManager& operator=(const GameManager&) = delete;
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
};