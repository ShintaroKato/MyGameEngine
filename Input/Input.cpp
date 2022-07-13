#include "d3dx12.h"
#include "Input.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

Input* Input::GetInstance()
{
	static Input instance;

	return &instance;
}

void Input::Initialize(WinApp* winApp)
{
	this->winApp_ = winApp;

	HRESULT result;

	result = DirectInput8Create(
		winApp_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);

	// キーボードデバイスの生成
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard); // 標準形式
	result = devkeyboard->SetCooperativeLevel(
		winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	// マウスデバイスの生成	
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);
	result = devMouse->SetDataFormat(&c_dfDIMouse2); // 標準形式
	result = devMouse->SetCooperativeLevel(
		winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

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

	//マウス情報の取得開始
	result = devMouse->Acquire();
	//前回の入力を保存
	mouseStatePre = mouseState;
	//マウスの入力情報を取得
	result = devMouse->GetDeviceState(sizeof(mouseState), &mouseState);
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

bool Input::PushMouse(MouseButton button)
{
	// 0でなければ押している
	if (mouseState.rgbButtons[button]) {
		return true;
	}

	// 押していない
	return false;
}

bool Input::TriggerMouse(MouseButton button)
{
	// 前回が0で、今回が0でなければトリガー
	if (!mouseStatePre.rgbButtons[button] && mouseState.rgbButtons[button]) {
		return true;
	}

	// トリガーでない
	return false;
}

XMFLOAT2 Input::GetMousePos2()
{
	POINT p;

	GetCursorPos(&p);

	ScreenToClient(winApp_->GetHwnd(), &p);

	return XMFLOAT2({ p.x,p.y });
}

XMFLOAT3 Input::GetMousePos3(XMMATRIX viewProjectionMat)
{
	XMFLOAT2 p = GetMousePos2();

	XMMATRIX invVPMat = XMMatrixInverse(&XMVECTOR({1,1,1}), viewProjectionMat);

	return XMFLOAT3();
}
