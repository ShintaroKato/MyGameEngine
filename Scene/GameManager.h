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
	// �Q�[���X�^�[�g
	static void Start();
	// �Q�[�����X�^�[�g
	static void Restart();
	// �X�V
	static void Update();
	// �`��
	static void Draw();
	// �G�l�~�[���폜
	static void DeleteEnemy();
	// �I���t���O�̏�Ԏ擾
	static int GetFinishState() { return finish; }
	// �^�C�}�[�擾
	static int GetTimer() { return timer; }
	// �^�C�}�[�擾(�b�Ɋ��Z)
	static int GetTimerSeconds() { return timer / 60; }
	// ���܂łɓ|�����G�̍��v���擾
	static int GetEnemyDefeatCount() { return score; }
	// �G�̐��̍ő�l���擾
	static int GetEnemyCountMax() { return groupCountMax * enemyMaxAll; }
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
	// �G�l�~�[�̃��f����ۑ�(OBJ)
	static void SetEnemyModel(ModelOBJ* model[])
	{
		modelObjEnemy.clear();

		for (int i = 0; i < EnemyType::TYPE_COUNT; i++)
		{
			modelObjEnemy.push_back(model[i]);
		}
	}
	// �G�l�~�[�̃��f����ۑ�(FBX)
	static void SetEnemyModel(ModelFBX* model[])
	{
		modelFbxEnemy.clear();

		for (int i = 0; i < EnemyType::TYPE_COUNT; i++)
		{
			modelFbxEnemy.push_back(model[i]);
		}
	}

private:
	// ���x��
	static void Level();
	// �G�l�~�[�o��
	static bool Spawn();
	// �G�l�~�[����萔�̏W�c�Ƃ��ďo��������
	static void SpawnEnemyGroup();
	// �G�l�~�[���X�V
	static void UpdateEnemy();
private:
	// �G�I�u�W�F�N�g
	static std::forward_list<Enemy> enemies;
	static std::vector<ModelOBJ*> modelObjEnemy;
	static std::vector<ModelFBX*> modelFbxEnemy;
	static int spawn_dir;
	static float spawn_angle;
	// ���Ώۂ̃I�u�W�F�N�g
	static StageObject* stageObject;
	// �G�̈�̃O���[�v�ɂ�����^�C�v���Ƃ̏o�������
	static int enemyCountMax[EnemyType::TYPE_COUNT];
	// �G�̈�̃O���[�v�ɂ�����o��������̍��v
	static int enemyMaxAll;
	// �G���^�C�v���ƂɃJ�E���g
	static int enemyCount[EnemyType::TYPE_COUNT];
	// �G�����܂łɏo��������
	static int enemyCountTotal;
	// �G�̏W�c����������Ă���ďo������Ԋu
	static int groupRespawnTimer;
	static int groupRespawnTimerMax;
	// �G�̏W�c�̐�
	static int groupCount;
	static int groupCountMax;
	// ��̂��o������Ԋu
	static int enemySpawnTimer;
	static int enemySpawnTimerMax;
	static XMFLOAT3 spawnPos;
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

private:
	GameManager() = default;
	GameManager(const GameManager&) = delete;
	~GameManager() = default;
	GameManager& operator=(const GameManager&) = delete;
};