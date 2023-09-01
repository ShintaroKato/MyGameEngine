#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>

class Camera
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	/// <summary>
	/// �J����������
	/// </summary>
	/// <param name="window_width">��ʉ���</param>
	/// <param name="window_height">��ʏc��</param>
	static void Initialize(const int& window_width, const int& window_height);

	/// <summary>
	/// �X�V
	/// </summary>
	static void Update();

	/// <summary>
	/// �r���[�s����X�V
	/// </summary>
	static void UpdateViewMatrix();

	/// <summary>
	/// �ˉe�s����X�V
	/// </summary>
	static void UpdateProjectionMatrix();

	/// <summary>
	/// ���_���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	static const XMFLOAT3& GetEye() { return eye; }

	/// <summary>
	/// �r���[�s��擾
	/// </summary>
	static const XMMATRIX& GetViewMatrix() { return matView; }

	/// <summary>
	/// �ˉe�s��擾
	/// </summary>
	static const XMMATRIX& GetProjectionMatrix() { return matProjection; }
	
	/// <summary>
	/// �ˉe�s��擾
	/// </summary>
	static const XMMATRIX& GetViewProjectionMatrix() { return matView * matProjection; }

	/// <summary>
	/// �r���{�[�h�s��擾
	/// </summary>
	static const XMMATRIX& GetBillboardMatrix() { return matBillboard; }

	/// <summary>
	/// ���_���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	static void SetEye(XMFLOAT3 eye);

	/// <summary>
	/// ���_���W�̐ݒ�
	/// </summary>
	/// <param name="rotation">��]��</param>
	/// <param name="distance">����</param>
	static void SetEye(XMFLOAT3 rotation, float distance);

	/// <summary>
	/// �����_���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	static const XMFLOAT3& GetTarget() { return target; }

	/// <summary>
	/// �����_���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	static void SetTarget(XMFLOAT3 target);

	/// <summary>
	/// �x�N�g���ɂ��ړ�
	/// </summary>
	/// <param name="move">�ړ���</param>
	static void CameraMoveVector(XMFLOAT3 move);

	/// <summary>
	/// ���_����
	/// </summary>
	static void ControlCamera();

	/// <summary>
	/// �n�ʂƂ̏Փ˓_���v�Z
	/// </summary>
	static void CalcGroundCollisionPoint();

	/// <summary>
	/// �ǂƂ̏Փ˓_���v�Z
	/// </summary>
	static void CalcWallCollisionPoint();

	/// <summary>
	/// ��]�ʂ��擾
	/// </summary>
	static XMFLOAT3 GetRotation() { return rot; }

	/// <summary>
	/// ��]�ʂ�ݒ�
	/// </summary>
	static void SetRotation(XMFLOAT3 rotation = {}) { rot = rotation; }

	/// <summary>
	/// �J�����̈ړ����L�����ۂ���ݒ�
	/// </summary>
	static void SetCameraMoveFlag(bool flag) { cameraMoveActive = flag; }

	/// <summary>
	/// �J�����̑��삪�ł����Ԃ��ۂ���ݒ�
	/// </summary>
	static void SetCameraControlFlag(bool flag) { cameraControlActive = flag; }

	/// <summary>
	/// �J�����̑��삪�ł����Ԃ��ۂ����擾
	/// </summary>
	static bool GetCameraControlFlag() { return cameraControlActive; }

	/// <summary>
	/// �Q�[���{�҂��ۂ���ݒ�
	/// </summary>
	static void SetInGameFlag(bool flag) { isInGame = flag; }

	/// <summary>
	/// �^�[�Q�b�g�ƃJ�����̋���
	/// </summary>
	static void SetCameraDistance(float length) { distance = length; }

private:
	// ���_���W
	static XMFLOAT3 eye;
	// �����_���W
	static XMFLOAT3 target;
	// ������x�N�g��
	static XMFLOAT3 up;
	// �r���[�s��
	static XMMATRIX matView;
	// �ˉe�s��
	static XMMATRIX matProjection;
	// �r���[�ˉe�s��
	static XMMATRIX matViewProjection;
	// �r���{�[�h�s��
	static XMMATRIX matBillboard;
	// �A�X�y�N�g��
	static float aspect;

	// �J��������ON�EOFF
	static bool cameraMoveActive;
	static bool cameraControlActive;

	static XMFLOAT3 rot;
	static float distance;

	static float distMax;
	static float distMin;
	static float distSpeed;

	// �Q�[���{�҂��ۂ�
	static bool isInGame;
};