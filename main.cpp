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
#include "Input.h"
#include "DirectXCommon.h"
#include "Object3d.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "SceneManager.h"
#include "Audio.h"
#include "FBXLoader.h"

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

	HRESULT result;

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
	spriteCommon->Initialize(dxCommon->GetDev(), dxCommon->GetCmdList(), winApp->window_width, winApp->window_width);

	// 3Dオブジェクト静的初期化
	Object3d::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);

	// FBX初期化
	FBXLoader::GetInstance()->Initialize(dxCommon->GetDev());

	// シーン初期化
	SceneManager* sceneManager = nullptr;
	sceneManager = new SceneManager();
	sceneManager->Initialize(dxCommon, spriteCommon, input, audio);

#pragma endregion 描画初期化処理

	while (true)  // ゲームループ
	{
		if (winApp->ProcessMessage())
		{
			break;
		}

		// シーン更新
		input->Update();
		sceneManager->Update();
		// シーン描画
		sceneManager->Draw();
	}

	FBXLoader::GetInstance()->Finalize();
	delete sceneManager;

#pragma region WindowsAPI後始末
	winApp->Finalize();
	delete winApp;
#pragma endregion WindowsAPI後始末

	return 0;
}