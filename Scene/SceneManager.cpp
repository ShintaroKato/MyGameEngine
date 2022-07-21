#include "SceneManager.h"

int SceneManager::scene = TITLE;
SceneNum SceneManager::currentScene = TITLE;
bool SceneManager::isLoaded = false;

void SceneManager::SceneChange()
{
	scene++;

	if (scene >= NONE)
	{
		scene = TITLE;
	}

	currentScene = static_cast<SceneNum>(scene);
}

void SceneManager::SetScene(SceneNum scene)
{
	SceneManager::currentScene = scene;

	SceneManager::scene = currentScene;

	isLoaded = false;
}