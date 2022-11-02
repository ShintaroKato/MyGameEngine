#pragma once

class GameManager
{
public:
	GameManager();

	void Start();
	void Update();
	// �I���t���O�̏�Ԏ擾
	int GetFinishState() { return finish; }
	// �^�C�}�[�擾
	int GetTimer() { return timer; }
	// �^�C�}�[�擾(�b�Ɋ��Z)
	int GetTimerSeconds() { return timer / 60; }

private:
	// ��������
	int timerMax = 10 * 60;
	int timer = timerMax;
	// �X�R�A
	int score = 0;
	// �E�F�[�u��
	int wave = 1;
	int waveMax = 1;
	// �E�F�[�u�I��
	int finish = 0; // 1�ŃN���A���� -1�Ŏ��s����
};