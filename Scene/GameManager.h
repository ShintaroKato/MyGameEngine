#pragma once
#include "StageObject.h"

class GameManager
{
public:
	// �Q�[���X�^�[�g
	static void Start();
	// �Q�[�����X�^�[�g
	static void Restart();
	// �X�V
	static void Update();
	// �I���t���O�̏�Ԏ擾
	static int GetFinishState() { return finish; }
	// �^�C�}�[�擾
	static int GetTimer() { return timer; }
	// �^�C�}�[�擾(�b�Ɋ��Z)
	static int GetTimerSeconds() { return timer / 60; }
	// �ҋ@���Ԏ擾
	static int GetWaitTimer() { return waitTimer; }
	// �ҋ@���Ԏ擾(�b�Ɋ��Z)
	static int GetWaitTimerSeconds() { return waitTimer / 60; }
	// ���Ώۂ̃I�u�W�F�N�g���Z�b�g
	static void SetStageObject(StageObject* stageObject) { GameManager::stageObject = stageObject; }

private:
	GameManager() = default;
	GameManager(const GameManager&) = delete;
	~GameManager() = default;
	GameManager& operator=(const GameManager&) = delete;
	// �J�n�O�ҋ@����
	static const int waitTimerMax = 10 * 60;
	static int waitTimer;
	// ��������
	static const int timerMax = 60 * 60;
	static int timer;
	// �X�R�A
	static int score;
	// �E�F�[�u��
	static const int waveMax = 1;
	static int wave;
	// �E�F�[�u�I��
	static int finish; // 1�ŃN���A���� -1�Ŏ��s����
	// ���Ώۂ̃I�u�W�F�N�g
	static StageObject* stageObject;
};