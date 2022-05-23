#include "SceneManager.h"

int SceneManager::scene = TITLE;
SceneNum SceneManager::currentScene = TITLE;

void SceneManager::Initialize(DirectXCommon* dxCommon, SpriteCommon* spriteCommon, Input* input, Audio* audio)
{
	// nullptrチェック
	assert(dxCommon);
	assert(spriteCommon);
	assert(input);

	this->dxCommon_ = dxCommon;
	this->spriteCommon_ = spriteCommon;
	this->input_ = input;
	this->audio_ = audio;
}

void SceneManager::Update()
{

}

void SceneManager::Draw()
{

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
	scene = TITLE;

	currentScene = static_cast<SceneNum>(scene);
}