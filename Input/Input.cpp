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

	// �L�[�{�[�h�f�o�C�X�̐���
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard); // �W���`��
	result = devkeyboard->SetCooperativeLevel(
		winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	// �}�E�X�f�o�C�X�̐���	
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);
	result = devMouse->SetDataFormat(&c_dfDIMouse2); // �W���`��
	result = devMouse->SetCooperativeLevel(
		winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	result = dinput->CreateDevice(GUID_Joystick, &devJoystick, NULL);
	// �f�o�C�X�̃t�H�[�}�b�g�̐ݒ�
	if (devJoystick != nullptr)
	{
		InitJoyStick(winApp_->GetHwnd());
	}
}

void Input::Update()
{
	HRESULT result;

	//�L�[�{�[�h���̎擾�J�n
	result = devkeyboard->Acquire();
	//�O��̃L�[���͂�ۑ�
	memcpy(keyPre, key, sizeof(key));
	//�S�L�[�̓��͏����擾
	result = devkeyboard->GetDeviceState(sizeof(key), key);

	//�}�E�X���̎擾�J�n
	result = devMouse->Acquire();
	//�O��̓��͂�ۑ�
	mouseStatePre = mouseState;
	//�}�E�X�̓��͏����擾
	result = devMouse->GetDeviceState(sizeof(mouseState), &mouseState);

	//�p�b�h���ڑ�����Ă��邩�𔻕�
	if (devJoystick == nullptr && isJoyStickConnect == true)
	{
		isJoyStickConnect = false;
	}
	if (isJoyStickConnect == false && count == 100)//100�t���[����1�񔻒�
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

		//�Q�[���p�b�h
		result = devJoystick->Acquire();
		// �O��̓��͂�ۑ�
		joyStatePre = joyState;
		//�Q�[���p�b�h�̓��͏����擾
		result = devJoystick->GetDeviceState(sizeof(joyState), &joyState);

		// ���s���̑Ή�
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

	//�w��L�[�������Ă����true
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
	// 0�łȂ���Ή����Ă���
	if (mouseState.rgbButtons[button]) {
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool Input::TriggerMouse(MouseButton button)
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!mouseStatePre.rgbButtons[button] && mouseState.rgbButtons[button]) {
		return true;
	}

	// �g���K�[�łȂ�
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
	// �O��0�ŁA����0�łȂ����
	if (joyState.rgbButtons[buttonNumber]) {
		return true;
	}

	return false;
}

bool Input::TriggerButton(BYTE buttonNumber)
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!joyStatePre.rgbButtons[buttonNumber] && joyState.rgbButtons[buttonNumber]) {
		return true;
	}

	return false;
}

bool Input::PushCrossButton(int buttonNumber)
{
	// �O��0�ŁA����0�łȂ����
	if (joyState.rgdwPOV[0] == buttonNumber) {
		return true;
	}

	return false;
}

bool Input::TriggerCrossButton(int buttonNumber)
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
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

	if (joyState.rgdwPOV[0] == 0xFFFFFFFF) return INPUT_NONE;//�����Ȓl�ł����NONE(����������Ă��Ȃ����)��Ԃ�

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

	if (button == INPUT_NONE) return button;//4�̃t���O���S��false�Ȃ�NONE(����������Ă��Ȃ����)��Ԃ�

	//�΂ߕ����̓��͂𔻕�
	if (isPushCrossButton[UP])
	{
		if (isPushCrossButton[RIGHT])
		{
			button = UP_RIGHT;//�E��
		}
		else if (isPushCrossButton[LEFT])
		{
			button = UP_LEFT;//����
		}
	}
	else if (isPushCrossButton[DOWN])
	{
		if (isPushCrossButton[RIGHT])
		{
			button = DOWN_RIGHT;//�E��
		}
		else if (isPushCrossButton[LEFT])
		{
			button = DOWN_LEFT;//����
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

XMVECTOR Input::CalcScreenToWorld(XMFLOAT2 scrPos, float projZ, // �ˉe��Ԃł�Z�l�i0�`1�j
	XMMATRIX view, XMMATRIX prj)
{
	XMVECTOR vec{};

	// �e�s��̋t�s����Z�o
	XMMATRIX invView = XMMatrixInverse(nullptr, view);
	XMMATRIX invPrj = XMMatrixInverse(nullptr, prj);
	XMMATRIX VP = XMMatrixIdentity();
	VP.r[0].m128_f32[0] = WinApp::window_width / 2.0f;
	VP.r[1].m128_f32[1] = -WinApp::window_height / 2.0f;
	VP.r[3].m128_f32[0] = WinApp::window_width / 2.0f;
	VP.r[3].m128_f32[1] = WinApp::window_height / 2.0f;
	XMMATRIX invViewport = XMMatrixInverse(nullptr, VP);

	// �t�ϊ�
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

	// XZ���ʂƌ������Ă��Ȃ��ꍇ�͉�����XY���ʂƂ̌�_���o��
	if (ray.m128_f32[1] <= 0)
	{
		// ����_
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

	// �����[�h���Βl���[�h�Ƃ��Đݒ�
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;	// ��Βl���[�h�̎w��(DIPROPAXISMODE_REL�ɂ����瑊�Βl)

	result = devJoystick->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	// X���̒l�͈̔͐ݒ�
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.diph.dwObj = DIJOFS_X;
	diprg.lMin = -1000;
	diprg.lMax = 1000;
	result = devJoystick->SetProperty(DIPROP_RANGE, &diprop.diph);


	// Y���̒l�͈̔͐ݒ�
	diprg.diph.dwObj = DIJOFS_Y;
	result = devJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);

	// �������[�h�̐ݒ�
	result = devJoystick->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
}