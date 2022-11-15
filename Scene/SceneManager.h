#pragma once
#include "SceneTitle.h"
#include "SceneStageEdit.h"
#include "SceneInGame.h"

enum SceneNum
{
	TITLE,
	EDIT,
	GAME,
	SCENE_NONE
};

class SceneManager
{
private:

	static SceneNum currentScene;

public:
	// �V�[������
	static auto CreateScene();
	// ������
	static void Initialize(DirectXCommon* dxCommon, SpriteCommon* spriteCommon, Input* input, Audio* audio);
	// �X�V
	static void Update();
	// �`��
	static void Draw();
	// �V�[���폜
	static void DeleteScene();

	/// <summary>
	/// �V�[����؂�ւ���
	/// </summary>
	static void SceneChange();

	/// <summary>
	/// �V�[�����𒼐ړn���Đ؂�ւ���
	/// </summary>
	static void SetScene(SceneNum scene);

	/// <summary>
	/// �V�[���̔ԍ����擾
	/// </summary>
	static SceneNum GetScene() { return currentScene; }

	/// <summary>
	/// �������ς݂ł��邩�̔�����擾
	/// </summary>
	static bool GetInitFlag()
	{
		if(scenes[currentScene]) return true;
		else return false;
	}

private:
	SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	~SceneManager() = default;
	SceneManager& operator=(const SceneManager&) = delete;

	// �V�[��������
	static SceneTitle* title;
	static SceneStageEdit* edit;
	static SceneInGame* game;
	static SceneBase* scenes[];
};