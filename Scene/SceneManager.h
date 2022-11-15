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
	// シーン生成
	static auto CreateScene();
	// 初期化
	static void Initialize(DirectXCommon* dxCommon, SpriteCommon* spriteCommon, Input* input, Audio* audio);
	// 更新
	static void Update();
	// 描画
	static void Draw();
	// シーン削除
	static void DeleteScene();

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

	// シーン初期化
	static SceneTitle* title;
	static SceneStageEdit* edit;
	static SceneInGame* game;
	static SceneBase* scenes[];
};