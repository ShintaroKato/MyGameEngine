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

	result = dinput->CreateDevice(GUID_Joystick, &devJoystick, NULL);
	// デバイスのフォーマットの設定
	if (devJoystick != nullptr)
	{
		InitJoyStick(winApp_->GetHwnd());
	}
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

	//パッドが接続されているかを判別
	if (devJoystick == nullptr && isJoyStickConnect == true)
	{
		isJoyStickConnect = false;
	}
	if (isJoyStickConnect == false && count == 100)//100フレームに1回判定
	{
		result = dinput->CreateDevice(GUID_Joystick, &devJoystick, NULL);
	}
	if (devJoystick != nullptr)
	{
		if (isJoyStickConnect == false)
		{
			InitJoyStick(winApp_->GetHwnd());

			isJoyStickConnect = true;
		}

		//ゲームパッド
		result = devJoystick->Acquire();
		// 前回の入力を保存
		joyStatePre = joyState;
		//ゲームパッドの入力情報を取得
		result = devJoystick->GetDeviceState(sizeof(joyState), &joyState);

		// 失敗時の対応
		result = devJoystick->GetDeviceState(sizeof(DIJOYSTATE), &joyState);
		if (FAILED(result))
		{
			devJoystick->Poll();
		}
	}

	if (count == 100)
	{
		count = 0;
	}
	count++;

	return;
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

XMFLOAT3 Input::GetMouseMovement()
{
	XMFLOAT3 move = {
		(float)mouseState.lX,
		(float)mouseState.lY,
		(float)mouseState.lZ
	};

	return move;
}

bool Input::ReleaseMouse(MouseButton button)
{
	if (mouseStatePre.rgbButtons[button] && !mouseState.rgbButtons[button]) {
		return true;
	}

	return false;
}

bool Input::PushButton(BYTE buttonNumber)
{
	// 前回が0で、今回が0でなければ
	if (joyState.rgbButtons[buttonNumber]) {
		return true;
	}

	return false;
}

bool Input::TriggerButton(BYTE buttonNumber)
{
	// 前回が0で、今回が0でなければトリガー
	if (!joyStatePre.rgbButtons[buttonNumber] && joyState.rgbButtons[buttonNumber]) {
		return true;
	}

	return false;
}

bool Input::PushCrossButton(int buttonNumber)
{
	// 前回が0で、今回が0でなければ
	if (joyState.rgdwPOV[0] == buttonNumber) {
		return true;
	}

	return false;
}

bool Input::TriggerCrossButton(int buttonNumber)
{
	// 前回が0で、今回が0でなければトリガー
	if (!joyStatePre.rgdwPOV[0] == -1 && joyState.rgdwPOV[0] == buttonNumber) {
		return true;
	}

	return false;
}

int Input::CheckCrossButton(DIJOYSTATE2 joyState)
{
	for (int i = 0; i < 4; i++)
	{
		isPushCrossButton[i] = false;
	}

	if (joyState.rgdwPOV[0] == 0xFFFFFFFF) return INPUT_NONE;//無効な値であればNONE(何も押されていない状態)を返す

	CrossButton button = CrossButton::INPUT_NONE;

	float rad = XMConvertToRadians(joyState.rgdwPOV[0] / 100.0f);
	float x = sin(rad);
	float y = cos(rad);

	if (x < -0.01f)
	{
		isPushCrossButton[LEFT] = true;
		button = LEFT;
	}
	else if (x > 0.01f)
	{
		isPushCrossButton[RIGHT] = true;
		button = RIGHT;
	}

	if (y > 0.01f)
	{
		isPushCrossButton[UP] = true;
		button = UP;
	}
	else if (y < -0.01f)
	{
		isPushCrossButton[DOWN] = true;
		button = DOWN;
	}

	if (button == INPUT_NONE) return button;//4つのフラグが全てfalseならNONE(何も押されていない状態)を返す

	//斜め方向の入力を判別
	if (isPushCrossButton[UP])
	{
		if (isPushCrossButton[RIGHT])
		{
			button = UP_RIGHT;//右上
		}
		else if (isPushCrossButton[LEFT])
		{
			button = UP_LEFT;//左上
		}
	}
	else if (isPushCrossButton[DOWN])
	{
		if (isPushCrossButton[RIGHT])
		{
			button = DOWN_RIGHT;//右下
		}
		else if (isPushCrossButton[LEFT])
		{
			button = DOWN_LEFT;//左下
		}
	}

	return button;
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

void Input::InitJoyStick(HWND hwnd)
{
	HRESULT result;

	result = devJoystick->SetDataFormat(&c_dfDIJoystick);

	// 軸モードを絶対値モードとして設定
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;	// 絶対値モードの指定(DIPROPAXISMODE_RELにしたら相対値)

	result = devJoystick->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	// X軸の値の範囲設定
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.diph.dwObj = DIJOFS_X;
	diprg.lMin = -1000;
	diprg.lMax = 1000;
	result = devJoystick->SetProperty(DIPROP_RANGE, &diprop.diph);


	// Y軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_Y;
	result = devJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);

	// 協調モードの設定
	result = devJoystick->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
}