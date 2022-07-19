#include <Windows.h>
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
	ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif
	//ポインター置き場
	DirectXCommon* dxCommon = nullptr;
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	Input* input = nullptr;
	input = new Input();
	input->Initialize(winApp);

	Audio* audio = nullptr;
	audio = new Audio();
	audio->Initialize();

	// DirectX初期化処理　ここまで
#pragma endregion DirectX初期化処理

#pragma region 描画初期化処理

	//FbxManager* fbxManager = FbxManager::Create();

	// スプライト共通データ生成
	SpriteCommon* spriteCommon = nullptr;
	spriteCommon = new SpriteCommon();
	spriteCommon->Initialize(dxCommon->GetDev(), dxCommon->GetCmdList(), winApp->window_width, winApp->window_height);

	// ポストエフェクト初期化
	PostEffect* postEffect1 = nullptr;
	postEffect1 = PostEffect::Create();

	PostEffect* postEffect2 = nullptr;
	postEffect2 = PostEffect::Create();

	postEffect1->SetTexSize({ 0.7f, 0.7f });
	postEffect2->SetTexSize({ 0.9f, 0.9f });

	RenderTexture* renderTex = nullptr;
	renderTex = RenderTexture::Create();
	renderTex->SetTexSize({ 0.5f,0.5f });

	// 3Dオブジェクト静的初期化
	ObjectOBJ::StaticInitialize(dxCommon->GetDev());

	// FBX初期化
	FBXLoader::GetInstance()->Initialize(dxCommon->GetDev());
	ObjectFBX::StaticInitialize(dxCommon->GetDev());

	// シーン初期化
	SceneInGame* game = new SceneInGame();

	SceneBase* scene[] =
	{
		game,
	};
	for (int i = 0; i < _countof(scene); i++)
	{
		scene[i]->Initialize(dxCommon, spriteCommon, input, audio);
	}

	// ポストエフェクト用シーン初期化
	PostEffectScene* postEffectScene = new PostEffectScene();
	postEffectScene->Initialize(dxCommon, spriteCommon, input, audio);

#pragma endregion 描画初期化処理

	while (true)  // ゲームループ
	{
		if (winApp->ProcessMessage())
		{
			break;
		}

		// 更新
		input->Update();

		if (input->PushKey(DIK_J) && postEffect1->alpha < 1)
		{
			postEffect1->alpha += 0.01;
			postEffect2->alpha += 0.01;
		}
		if (input->PushKey(DIK_K) && postEffect1->alpha > 0)
		{
			postEffect1->alpha -= 0.01;
			postEffect2->alpha -= 0.01;
		}

		if (input->PushKey(DIK_LSHIFT))
		{
			if (input->TriggerKey(DIK_1))
			{
				postEffect2->mode = 0;
			}
			if (input->TriggerKey(DIK_2))
			{
				postEffect2->mode = 1;
			}
			if (input->TriggerKey(DIK_3))
			{
				postEffect2->mode = 2;
			}
			if (input->TriggerKey(DIK_4))
			{
				postEffect2->mode = 3;
			}
			if (input->TriggerKey(DIK_5))
			{
				postEffect2->mode = 4;
			}
		}
		else
		{
			if (input->TriggerKey(DIK_1))
			{
				postEffect1->mode = 0;
			}
			if (input->TriggerKey(DIK_2))
			{
				postEffect1->mode = 1;
			}
			if (input->TriggerKey(DIK_3))
			{
				postEffect1->mode = 2;
			}
			if (input->TriggerKey(DIK_4))
			{
				postEffect1->mode = 3;
			}
			if (input->TriggerKey(DIK_5))
			{
				postEffect1->mode = 4;
			}
		}

		if (input->TriggerKey(DIK_9))
		{
			postEffect1->SwitchDraw();
		}
		if (input->TriggerKey(DIK_0))
		{
			postEffect2->SwitchDraw();
		}

		// シーン更新
		scene[SceneManager::GetScene()]->Update();
		postEffectScene->Update();

		// レンダーテクスチャへの描画
		postEffect1->PreDrawScene(dxCommon->GetCmdList());
		postEffectScene->Draw();
		postEffect1->PostDrawScene(dxCommon->GetCmdList());

		postEffect2->PreDrawScene(dxCommon->GetCmdList());
		postEffect1->Draw();
		postEffect2->PostDrawScene(dxCommon->GetCmdList());

		//描画開始
		dxCommon->PreDraw();
		// シーン描画
		scene[SceneManager::GetScene()]->Draw();

		// ポストエフェクト
		postEffect2->Draw();
		//描画終了
		dxCommon->PostDraw();
	}

	delete postEffect1;
	delete postEffect2;

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