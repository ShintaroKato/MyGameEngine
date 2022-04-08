#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include "Input.h"

void Input::Initialize(WinApp* winApp)
{
	this->winApp = winApp;

	HRESULT result;

	result = DirectInput8Create(
		winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);

	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard); // 標準形式
	result = devkeyboard->SetCooperativeLevel(
		winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
}

void Input::Update()
{
	HRESULT result;

	//キーボード情報の取得開始
	result = devkeyboard->Acquire();
	//前回のキー入力を保存
	memcpy(keyPre, key, sizeof(key));

	//全キーの入力情報を取得
	result = devkeyboard->GetDeviceState(sizeof(key), key);
}

bool Input::PushKey(BYTE keyNumber)
{
	//指定キーを押していればtrue
	if (key[keyNumber])
	{
		return true;
	}
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	if (!keyPre[keyNumber] && key[keyNumber])
	{
		return true;
	}
	return false;
}
