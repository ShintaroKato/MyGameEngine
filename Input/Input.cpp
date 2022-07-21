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
