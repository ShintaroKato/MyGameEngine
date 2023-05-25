#include "Camera.h"
#include "Input.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "ParticleEmitter.h"

using namespace DirectX;

XMMATRIX Camera::matView{};
XMMATRIX Camera::matProjection{};
XMMATRIX Camera::matBillboard{};
XMFLOAT3 Camera::eye = { 0, 5.0f, -50.0f };
XMFLOAT3 Camera::target = { 0, 0, 0 };
XMFLOAT3 Camera::up = { 0, 1, 0 };
float Camera::aspect;
bool Camera::cameraControlActive = false;
bool Camera::cameraMoveActive = false;
bool Camera::isInGame = false;
XMFLOAT3 Camera::rot{};
float Camera::distance = 10;
float Camera::distMax = 160.0f;
float Camera::distMin = 10.0f;
float Camera::distSpeed = 5.0f;

XMVECTOR operator-(XMFLOAT3 l, XMFLOAT3 r)
{
	XMVECTOR result{};
	result.m128_f32[0] = l.x - r.x;
	result.m128_f32[1] = l.y - r.y;
	result.m128_f32[2] = l.z - r.z;

	return result;
}

void Camera::Initialize(const int& window_width, const int& window_height)
{
	aspect = (float)window_width / window_height;

	Update();
}

void Camera::Update()
{
	ControlCamera();

	UpdateViewMatrix();

	UpdateProjectionMatrix();
}

void Camera::UpdateViewMatrix()
{
	// �r���[�s��̍X�V

	// ���_���W
	XMVECTOR eyePosition = XMLoadFloat3(&eye);
	// �����_���W
	XMVECTOR targetPosition = XMLoadFloat3(&target);
	// �i���́j�����
	XMVECTOR upVector = XMLoadFloat3(&up);

	// �J����Z���i���������j
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
	// 0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));
	// �x�N�g���𐳋K��
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	// �J������X���i�E�����j
	XMVECTOR cameraAxisX;
	// X���͏������Z���̊O�ςŋ��܂�
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	// �x�N�g���𐳋K��
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	// �J������Y���i������j
	XMVECTOR cameraAxisY;
	// Y����Z����X���̊O�ςŋ��܂�
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	// �����܂łŒ�������3�����̃x�N�g��������
	//�i���[���h���W�n�ł̃J�����̉E�����A������A�O�����j	

	// �J������]�s��
	XMMATRIX matCameraRot;
	// �J�������W�n�����[���h���W�n�̕ϊ��s��
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
	// �]�u�ɂ��t�s��i�t��]�j���v�Z
	matView = XMMatrixTranspose(matCameraRot);

	// ���_���W��-1���|�������W
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	// �J�����̈ʒu���烏�[���h���_�ւ̃x�N�g���i�J�������W�n�j
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);	// X����
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);	// Y����
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);	// Z����
	// ��̃x�N�g���ɂ܂Ƃ߂�
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	// �r���[�s��ɕ��s�ړ�������ݒ�
	matView.r[3] = translation;

	// �r���{�[�h�s��
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
}

void Camera::UpdateProjectionMatrix()
{
	// �������e�ɂ��ˉe�s��̐���
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		aspect,
		0.1f, 1000.0f
	);
}

void Camera::SetEye(XMFLOAT3 eye)
{
	Camera::eye = eye;

	UpdateViewMatrix();
}

void Camera::SetTarget(XMFLOAT3 target)
{
	Camera::target = target;

	UpdateViewMatrix();
}

void Camera::CameraMoveVector(XMFLOAT3 move)
{
	XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	target_moved.x += move.x;
	target_moved.y += move.y;
	target_moved.z += move.z;

	SetEye(eye_moved);
	SetTarget(target_moved);
}

void Camera::ControlCamera()
{
	if (!cameraControlActive) return;

	Input* input = Input::GetInstance();

	if (cameraMoveActive)
	{
		//�}�E�X����
		rot.x += input->GetMouseMovement().y * 0.025f;		//�㉺�ړ�
		rot.y -= input->GetMouseMovement().x * 0.025f;		//���E�ړ�
	}

	if (rot.x < -70)
	{
		rot.x = -70;
	}
	if (rot.x > 70)
	{
		rot.x = 70;
	}

	//�L�[�{�[�h����
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) ||
		input->PushKey(DIK_LEFT) || input->PushKey(DIK_RIGHT))
	{

		if (input->PushKey(DIK_UP))
		{
			rot.x--;
		}
		else if (input->PushKey(DIK_DOWN))
		{
			rot.x++;
		}
		if (input->PushKey(DIK_LEFT))
		{
			rot.y++;
		}
		else if (input->PushKey(DIK_RIGHT))
		{
			rot.y--;
		}
	}

	eye.x = target.x + distance * cos(XMConvertToRadians(rot.y - 90)) * cos(XMConvertToRadians(rot.x));
	eye.z = target.z + distance * sin(XMConvertToRadians(rot.y - 90)) * cos(XMConvertToRadians(rot.x));

	eye.y = target.y + 2 + distance * sin(XMConvertToRadians(rot.x));


	if(isInGame)
	{
		distMax = 6.0f;
		distMin = 5.0f;
		distSpeed = 0.1f;
	}
	else
	{
		distMax = 160.0f;
		distMin = 10.0f;
		distSpeed = 5.0f;
	}

	if (input->GetMouseMovement().z > 0 && distance > distMin)
	{
		distance -= distSpeed;
	}
	if (input->GetMouseMovement().z < 0 && distance <= distMax)
	{
		distance += distSpeed;
	}

	eye = CalcWallCollisionPoint();
}

XMFLOAT3 Camera::CalcWallCollisionPoint()
{
	Ray ray{};
	ray.start = XMLoadFloat3(&target);
	ray.dir = XMVector3Normalize(eye - target);
	RaycastHit raycast{};

	XMFLOAT3 pos{};

	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_OBJECT_MESH, &raycast, distance))
	{
		pos = {
			raycast.inter.m128_f32[0],
			raycast.inter.m128_f32[1],
			raycast.inter.m128_f32[2] };

	}
	else if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycast, distance))
	{
		pos = {
			raycast.inter.m128_f32[0],
			raycast.inter.m128_f32[1] + 0.05f,
			raycast.inter.m128_f32[2] };
	}
	else
	{
		pos = eye;
	}

	return pos;
}
