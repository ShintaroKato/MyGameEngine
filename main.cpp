﻿#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#include <d3dx12.h>

#include <vector>
#include <string>
#include <fstream>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include <DirectXTex.h>
#include <wrl.h>

#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")


#include "WinApp.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Input.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "Camera.h"
#include "ObjectOBJ.h"
#include "FBXLoader.h"
#include "SceneManager.h"
#include "PostEffect.h"
#include "PostEffectScene.h"
#include "RenderTexture.h"

using namespace DirectX;
using namespace Microsoft::WRL;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

#pragma region WindowsAPI初期化
	WinApp* winApp = nullptr;
	winApp = new WinApp();

	winApp->Initialize();
#pragma endregion WindowsAPI初期化

#pragma region DirectX初期化処理
	// DirectX初期化処理　ここから
#ifdef _DEBUG
	//デバッグレイヤーをオンに
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}

	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&infoQueue))))
	{
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
	}

#endif
	//ポインター置き場
	DirectXCommon* dxCommon = nullptr;
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	Input* input = nullptr;
	input = Input::GetInstance();
	input->Initialize(winApp);

	Audio* audio = nullptr;
	audio = new Audio();
	audio->Initialize();

	// DirectX初期化処理　ここまで
#pragma endregion DirectX初期化処理

#pragma region 描画初期化処理

	// スプライト共通データ生成
	SpriteCommon* spriteCommon = nullptr;
	spriteCommon = new SpriteCommon();
	spriteCommon->Initialize(dxCommon->GetDev(), dxCommon->GetCmdList(), winApp->window_width, winApp->window_height);

	// ポストエフェクト生成
	PostEffect* postEffect = nullptr;
	postEffect = PostEffect::Create();
	postEffect->SetTexSize({ 1,1 });

	// ポストエフェクト用シーン生成
	PostEffectScene* postEffectScene = new PostEffectScene();

	//// 3Dオブジェクト静的初期化
	ObjectOBJ::StaticInitialize(dxCommon->GetDev());

	// FBX初期化
	FBXLoader::GetInstance()->Initialize(dxCommon->GetDev());
	ObjectFBX::StaticInitialize(dxCommon->GetDev());

	// シーン初期化
	SceneTitle* title = new SceneTitle();
	SceneStageEdit* edit = new SceneStageEdit();
	SceneInGame* game = new SceneInGame();

	SceneBase* scene[] =
	{
		title,
		edit,
		game,
	};
	for (int i = 0; i < _countof(scene); i++)
	{
		scene[i]->Initialize(dxCommon, spriteCommon, input, audio);
	}

	postEffectScene->Initialize(dxCommon, spriteCommon, input, audio);

#pragma endregion 描画初期化処理

	while (true)  // ゲームループ
	{
		if (winApp->ProcessMessage())
		{
			break;
		}

		// 初期化
		if (!SceneManager::GetInitFlag())
		{
			scene[SceneManager::GetScene()]->Initialize(dxCommon, spriteCommon, input, audio);
			SceneManager::ChangeLoaded();
		}

		// 更新
		input->Update();

		// シーン更新
		scene[SceneManager::GetScene()]->Update();
		postEffectScene->Update();

		//描画


		if (!SceneManager::GetInitFlag())
		{
			postEffect->PreDrawScene(dxCommon->GetCmdList());
			postEffectScene->Draw();
			postEffect->PostDrawScene(dxCommon->GetCmdList());
		}

		dxCommon->PreDraw();
		// シーン描画
		scene[SceneManager::GetScene()]->Draw();

		if (!SceneManager::GetInitFlag())
		{
			postEffect->Draw();
		}

		dxCommon->PostDraw();


	}

	FBXLoader::GetInstance()->Finalize();

	int sceneCount = _countof(scene);

	for (int i = 0; i < sceneCount; i++)
	{
		delete scene[i];
	}

#pragma region WindowsAPI後始末
	winApp->Finalize();
	delete winApp;
#pragma endregion WindowsAPI後始末

	return 0;
}