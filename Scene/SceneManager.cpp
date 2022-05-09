#include "SceneManager.h"

int SceneManager::scene = TITLE;
SceneNum SceneManager::currentScene = TITLE;

void SceneManager::Initialize(DirectXCommon* dxCommon, SpriteCommon* spriteCommon, Input* input)
{
	// nullptr�`�F�b�N
	assert(dxCommon);
	assert(spriteCommon);
	assert(input);

	this->dxCommon_ = dxCommon;
	this->spriteCommon_ = spriteCommon;
	this->input_ = input;

	titleScene = new TitleScene();
	titleScene->Initialize(dxCommon_, spriteCommon_, input_);

	gameScene = new GameScene();
	gameScene->Initialize(dxCommon_, spriteCommon_, input_);
}

void SceneManager::Update()
{
	switch (currentScene)
	{
	case TITLE:
		titleScene->Update();
		break;

	case GAME_MAIN:
		gameScene->Update();
		break;
	default:
		break;
	}
}

void SceneManager::Draw()
{
	switch (currentScene)
	{
	case TITLE:
		titleScene->Draw();
		break;

	case GAME_MAIN:
		gameScene->Draw();
		break;
	default:
		break;
	}
}

void SceneManager::SceneChange()
{
	scene++;
	if (scene >= NONE)
	{
		scene = TITLE;
	}

	currentScene = static_cast<SceneNum>(scene);
}

void SceneManager::SceneChangeTitle()
{
	currentScene = TITLE;
}