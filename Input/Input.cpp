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

bool Input::ReleaseMouse(MouseButton button)
{
	if (mouseStatePre.rgbButtons[button] && !mouseState.rgbButtons[button]) {
		return true;
	}

	return false;
}

XMFLOAT2 Input::GetMousePos2()
{
	POINT p;

	GetCursorPos(&p);

	ScreenToClient(winApp_->GetHwnd(), &p);

	return XMFLOAT2({ (float)p.x,(float)p.y });
}

XMVECTOR Input::CursorPoint3D(XMMATRIX viewMat, XMMATRIX prjMat)
{
	XMFLOAT2 cursor = GetMousePos2();
	XMVECTOR point{};

	point = CalcScreenToXZ(cursor, viewMat, prjMat);

	return point;
}

XMVECTOR Input::CalcScreenToWorld(XMFLOAT2 scrPos, float projZ, // 射影空間でのZ値（0～1）
	XMMATRIX view, XMMATRIX prj)
{
	XMVECTOR vec{};

	// 各行列の逆行列を算出
	XMMATRIX invView = XMMatrixInverse(nullptr, view);
	XMMATRIX invPrj = XMMatrixInverse(nullptr, prj);
	XMMATRIX VP = XMMatrixIdentity();
	VP.r[0].m128_f32[0] = WinApp::window_width / 2.0f;
	VP.r[1].m128_f32[1] = -WinApp::window_height / 2.0f;
	VP.r[3].m128_f32[0] = WinApp::window_width / 2.0f;
	VP.r[3].m128_f32[1] = WinApp::window_height / 2.0f;
	XMMATRIX invViewport = XMMatrixInverse(nullptr, VP);

	// 逆変換
	XMMATRIX tmp = invViewport * invPrj * invView;
	vec = XMVector3TransformCoord(XMVECTOR({ scrPos.x, scrPos.y, projZ }), tmp);

	return vec;
}

XMVECTOR Input::CalcScreenToXZ(XMFLOAT2 scrPos, XMMATRIX view, XMMATRIX prj)
{
	XMVECTOR nearPos;
	XMVECTOR farPos;
	XMVECTOR ray;
	nearPos = CalcScreenToWorld(scrPos, 0.0f, view, prj);
	farPos = CalcScreenToWorld(scrPos, 1.0f, view, prj);
	ray = farPos - nearPos;
	ray = XMVector3Normalize(ray);

	// XZ平面と交差していない場合は遠くのXY平面との交点を出力
	if (ray.m128_f32[1] <= 0)
	{
		// 床交点
		XMVECTOR Lray = XMVector3Dot(ray, XMVECTOR({ 0, 1, 0 }));
		XMVECTOR LP0 = XMVector3Dot(XMVECTOR({0,0,0}) - nearPos, XMVECTOR({ 0, 1, 0 }));
		return nearPos + (LP0 / Lray) * ray;
	}
	else
	{
		return farPos;
	}
}
