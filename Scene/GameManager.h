#pragma once

class GameManager
{
public:
	GameManager();

	void Start();
	void Update();
	// 終了フラグの状態取得
	int GetFinishState() { return finish; }
	// タイマー取得
	int GetTimer() { return timer; }
	// タイマー取得(秒に換算)
	int GetTimerSeconds() { return timer / 60; }

private:
	// 制限時間
	int timerMax = 10 * 60;
	int timer = timerMax;
	// スコア
	int score = 0;
	// ウェーブ数
	int wave = 1;
	int waveMax = 1;
	// ウェーブ終了
	int finish = 0; // 1でクリア判定 -1で失敗判定
};