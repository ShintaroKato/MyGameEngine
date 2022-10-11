#pragma once
#include <wrl.h>
#include <Windows.h>
#define DIRECTINPUT_VERSION		0x0800 // DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include <DirectXMath.h>
#include "WinApp.h"

using namespace DirectX;

#pragma once

#include <Windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION     0x0800          // DirectInput�̃o�[�W�����w��
#include <dinput.h>

#define STICK_X_DEFAULT 32767	//�X�e�B�b�N(X����)�̒���
#define STICK_Y_DEFAULT 0		//�X�e�B�b�N(Y����)�̒���

/// <summary>
/// �{�^��_(XBox)_(DualShock4)
/// </summary>
enum ButtonNum
{
	Button_A_Cross,
	Button_B_Circle,
	Button_X_Square,
	Button_Y_Triangle,
	Button_LB_L1,
	Button_RB_R1,
	Button_LT_L2,
	Button_RT_R2,
	Button_Select,
	Button_Start,
	Button_StickL,
	Button_StickR,
};

enum Stick
{
	StickUp = 0,
	StickUpRight = 4500,
	StickRight = 9000,
	StickDownRight = 13500,
	StickDown = 18000,
	StickDownLeft = 22500,
	StickLeft = 27000,
	StickUpLeft = 31500,
};

enum CrossButton
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	UP_LEFT,
	DOWN_LEFT,
	UP_RIGHT,
	DOWN_RIGHT,
	INPUT_NONE,
};

enum MouseButton
{
	MOUSE_LEFT,
	MOUSE_RIGHT,
	MOUSE_MIDDLE,
};

/// <summary>
/// ����
/// </summary>
class Input
{
private:
	struct MouseMove {
		LONG    lX;
		LONG    lY;
		LONG    lZ;
	};

	struct JoyMove {
		LONG    lX;
		LONG    lY;
		LONG    lZ;
		LONG    lRx;
		LONG    lRy;
		LONG    lRz;
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

	//�Q�[���p�b�h�̃f�o�C�X
	ComPtr<IDirectInputDevice8> devJoystick;
	DIJOYSTATE2 joyState = {};
	DIJOYSTATE2 joyStatePre = {};

	bool isJoyStickConnect = false;
	int count = 0;

	bool isPushCrossButton[4]{};

	WinApp* winApp_ = nullptr;

public:
	static Input* GetInstance();
	void Initialize(WinApp* winApp);
	void Update();
	// �ǂ̃L�[��������Ă��邩
	bool PushKey(BYTE keyNumber);
	// �ǂ̃L�[�������ꂽ�u�Ԃ�
	bool TriggerKey(BYTE keyNumber);
	// �ǂ̃}�E�X�̃{�^����������Ă��邩
	bool PushMouse(MouseButton button);
	// �ǂ̃}�E�X�̃{�^���������ꂽ�u�Ԃ�
	bool TriggerMouse(MouseButton button);
	// �ǂ̃}�E�X�̃{�^���������ꂽ�u�Ԃ�
	bool ReleaseMouse(MouseButton button);
	// �ǂ̃p�b�h�̃{�^����������Ă��邩
	bool PushButton(BYTE buttonNumber);
	// �ǂ̃p�b�h�̃{�^���������ꂽ�u�Ԃ�
	bool TriggerButton(BYTE buttonNumber);

	bool PushCrossButton(int buttonNumber);
	bool TriggerCrossButton(int buttonNumber);
	int CheckCrossButton(DIJOYSTATE2 joyState);
	void InitJoyStick(HWND hwnd);
	XMFLOAT2 GetMousePos2();
	/// <summary>
	/// �}�E�X�J�[�\�����w������3D��Ԃ̍��W
	/// </summary>
	/// <param name="viewMat">�r���[�s��</param>
	/// <param name="prjMat">�ˉe�s��</param>
	XMVECTOR CursorPoint3D(XMMATRIX viewMat, XMMATRIX prjMat);

	XMVECTOR CalcScreenToWorld(XMFLOAT2 scrPos, float fZ, XMMATRIX view, XMMATRIX prj);
	XMVECTOR CalcScreenToXZ(XMFLOAT2 scrPos, XMMATRIX view, XMMATRIX prj);

	/// <summary>
	/// �}�E�X�̈ړ��ʂ��擾
	/// </summary>
	XMFLOAT3 GetMouseMovement();
};