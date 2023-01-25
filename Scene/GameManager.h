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
	// �Ō�̃E�F�[�u���ۂ��𔻒�
	static bool IsFinalWave() { return wave == waveMax; }
	// �E�F�[�u�̔ԍ����擾
	static int GetWaveNumber() { return wave; }
	// ���̃E�F�[�u�ɐ؂�ւ�
	static void ChangeNextWave();
	// ���x��
	static void Level();
	// ��x�ɏo������G�̐�
	static int GetEnemyCount() { return enemyMax; }
private:
	GameManager() = default;
	GameManager(const GameManager&) = delete;
	~GameManager() = default;
	GameManager& operator=(const GameManager&) = delete;
	// �J�n�O�ҋ@����
	static const int waitTimerMax = 5 * 60;
	static int waitTimer;
	// ��������
	static int timerMax;
	static int timer;
	// �X�R�A
	static int scoreTotal;
	static int score;
	// �E�F�[�u��
	static const int waveMax = 5;
	static int wave;
	// �E�F�[�u�I��
	static int finish; // 1�ŃN���A���� -1�Ŏ��s����
	// ���Ώۂ̃I�u�W�F�N�g
	static StageObject* stageObject;
	// ���x��
	static int level;
	static int enemyMax;
};