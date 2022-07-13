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
