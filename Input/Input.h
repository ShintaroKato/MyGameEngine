#pragma once
#include <wrl.h>
#include <Windows.h>
#define DIRECTINPUT_VERSION		0x0800 // DirectInput�̃o�[�W�����w��
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
	//�L�[�{�[�h�̃f�o�C�X
	ComPtr<IDirectInputDevice8> devkeyboard = nullptr;
	ComPtr<IDirectInput8> dinput = nullptr;

	//�}�E�X�̃f�o�C�X
	ComPtr<IDirectInputDevice8> devMouse;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 mouseStatePre = {};

	WinApp* winApp_ = nullptr;

public:
	static Input* GetInstance();
	void Initialize(WinApp* winApp);
	void Update();
	bool PushKey(BYTE keyNumber);
	bool TriggerKey(BYTE keyNumber);
	bool PushMouse(MouseButton button);
	bool TriggerMouse(MouseButton button);
	bool ReleaseMouse(MouseButton button);
	XMFLOAT2 GetMousePos2();
	/// <summary>
	/// �}�E�X�J�[�\�����w������3D��Ԃ̍��W
	/// </summary>
	/// <param name="viewMat">�r���[�s��</param>
	/// <param name="prjMat">�ˉe�s��</param>
	XMVECTOR CursorPoint3D(XMMATRIX viewMat, XMMATRIX prjMat);

	XMVECTOR CalcScreenToWorld(XMFLOAT2 scrPos, float fZ, XMMATRIX view, XMMATRIX prj);
	XMVECTOR CalcScreenToXZ(XMFLOAT2 scrPos, XMMATRIX view, XMMATRIX prj);

};