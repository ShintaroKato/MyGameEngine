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
	/// シーンを切り替える
	/// </summary>
	static void SceneChange();

	/// <summary>
	/// タイトル画面に戻る
	/// </summary>
	static void SceneChangeTitle();

	/// <summary>
	/// シーン名を直接渡して切り替える
	/// </summary>
	static void SetScene(SceneNum scene) { SceneManager::currentScene = scene; }

	/// <summary>
	/// シーンの番号を取得
	/// </summary>
	static SceneNum GetScene() { return currentScene; }
};