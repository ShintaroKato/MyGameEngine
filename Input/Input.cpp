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
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard); // �W���`��
	result = devkeyboard->SetCooperativeLevel(
		winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
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
