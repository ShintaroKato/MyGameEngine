#pragma once
#include <wrl.h>
#include <Windows.h>
#define DIRECTINPUT_VERSION		0x0800 // DirectInputのバージョン指定
#include <dinput.h>
#include <DirectXMath.h>
#include "WinApp.h"

using namespace DirectX;

enum MouseButton
{
	MOUSE_LEFT,
	MOUSE_RIGHT,
	MOUSE_MIDDLE,
};

class Input
{
private:
	struct MouseMove {
		LONG    lX;
		LONG    lY;
		LONG    lZ;
	};

public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};

private:
	//キーボードのデバイス
	ComPtr<IDirectInputDevice8> devkeyboard = nullptr;
	ComPtr<IDirectInput8> dinput = nullptr;

	//マウスのデバイス
	ComPtr<IDirectInputDevice8> devMouse;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 mouseStatePre = {};

	WinApp* winApp_ = nullptr;

public:
	static Input* Input::GetInstance();
	void Initialize(WinApp* winApp);
	void Update();
	bool PushKey(BYTE keyNumber);
	bool TriggerKey(BYTE keyNumber);
	bool PushMouse(MouseButton button);
	bool TriggerMouse(MouseButton button);
	XMFLOAT2 GetMousePos2();
	XMFLOAT3 GetMousePos3(XMMATRIX viewProjectionMat);
};