#pragma once
#include "SceneTitle.h"
#include "SceneInGame.h"

enum SceneNum
{
	TITLE,
	GAME_MAIN,
	GAME_OVER,
	GAME_CLEAR,
	NONE
};

class SceneManager
{
private:

	static int scene;
	static SceneNum currentScene;

public:
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
};