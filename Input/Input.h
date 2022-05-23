#pragma once
#include <wrl.h>
#include <Windows.h>
#define DIRECTINPUT_VERSION		0x0800 // DirectInputのバージョン指定
#include <dinput.h>
#include "WinApp.h"

class Input
{

public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};

private:
	//キーボードのデバイス
	ComPtr<IDirectInputDevice8> devkeyboard = nullptr;
	ComPtr<IDirectInput8> dinput = nullptr;

	WinApp* winApp_ = nullptr;

public:
	static Input* Input::GetInstance();
	void Initialize(WinApp* winApp);
	void Update();
	bool PushKey(BYTE keyNumber);
	bool TriggerKey(BYTE keyNumber);
};