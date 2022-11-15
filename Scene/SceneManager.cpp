#include "SceneManager.h"
#include "CollisionManager.h"

SceneNum SceneManager::currentScene = TITLE;

SceneTitle* SceneManager::title;
SceneStageEdit* SceneManager::edit;
SceneInGame* SceneManager::game;

SceneBase* SceneManager::scenes[] ={
	title,
	edit,
	game,
};


auto SceneManager::CreateScene()
{
	SceneBase* instance = new SceneBase();

	switch (currentScene)
	{
	case TITLE: instance = new SceneTitle();
		break;
	case EDIT: instance = new SceneStageEdit();
		break;
	case GAME: instance = new SceneInGame();
		break;
	}

	return instance;
}

void SceneManager::Initialize(DirectXCommon* dxCommon, SpriteCommon* spriteCommon, Input* input, Audio* audio)
{
	if (scenes[currentScene]) return;
	
	scenes[currentScene] = CreateScene();
	scenes[currentScene]->Initialize(dxCommon, spriteCommon, input, audio);
}

void SceneManager::Update()
{
	if(scenes[currentScene]) scenes[currentScene]->Update();
}

void SceneManager::Draw()
{
	if(scenes[currentScene]) scenes[currentScene]->Draw();
}

void SceneManager::DeleteScene()
{
	CollisionManager::GetInstance()->DeleteCollider();
	delete scenes[currentScene];
}

void SceneManager::SceneChange()
{
	DeleteScene();

	int scene_no = currentScene;
	scene_no++;
	currentScene = static_cast<SceneNum>(scene_no);

	if (currentScene >= SCENE_NONE)
	{
		currentScene = TITLE;
	}
}

void SceneManager::SetScene(SceneNum scene)
{
	DeleteScene();
	SceneManager::currentScene = scene;
}