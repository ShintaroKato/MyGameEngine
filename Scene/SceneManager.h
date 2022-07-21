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
	/// シーンを切り替える
	/// </summary>
	static void SceneChange();

	/// <summary>
	/// シーン名を直接渡して切り替える
	/// </summary>
	static void SetScene(SceneNum scene);

	/// <summary>
	/// シーンの番号を取得
	/// </summary>
	static SceneNum GetScene() { return currentScene; }

	/// <summary>
	/// 初期化済みであるかの判定を取得
	/// </summary>
	static bool GetInitFlag() { return isLoaded; }

	/// <summary>
	/// 初期化済みに変更
	/// </summary>
	static void ChangeLoaded() { isLoaded = true; }
};