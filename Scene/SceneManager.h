#pragma once
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Sprite.h"
#include "Text.h"
#include "Object3d.h"
#include "TitleScene.h"
#include "GameScene.h"

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
	/// ������
	/// </summary>
	void Initialize(DirectXCommon* dxCommon, SpriteCommon* spriteCommon, Input* input);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

public:
	/// <summary>
	/// �V�[����؂�ւ���
	/// </summary>
	static void SceneChange();

	/// <summary>
	/// �^�C�g����ʂɖ߂�
	/// </summary>
	static void SceneChangeTitle();

private:
	TitleScene* titleScene = nullptr;
	GameScene* gameScene = nullptr;

	DirectXCommon* dxCommon_ = nullptr;
	SpriteCommon* spriteCommon_ = nullptr;
	Input* input_ = nullptr;
};