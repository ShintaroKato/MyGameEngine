#pragma once
#include "SceneTitle.h"
#include "SceneStageEdit.h"
#include "SceneInGame.h"

enum SceneNum
{
	TITLE,
	EDIT,
	GAME,
	NONE
};

class SceneManager
{
private:

	static int scene;
	static SceneNum currentScene;
	static bool isLoaded;

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

	/// <summary>
	/// �������ς݂ł��邩�̔�����擾
	/// </summary>
	static bool GetInitFlag() { return isLoaded; }

	/// <summary>
	/// �������ς݂ɕύX
	/// </summary>
	static void ChangeLoaded() { isLoaded = true; }
};