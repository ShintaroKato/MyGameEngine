#include "SceneManager.h"

int SceneManager::scene = TITLE;
SceneNum SceneManager::currentScene = TITLE;

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