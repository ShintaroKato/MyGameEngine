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
	// ビュー行列の更新

	// 視点座標
	XMVECTOR eyePosition = XMLoadFloat3(&eye);
	// 注視点座標
	XMVECTOR targetPosition = XMLoadFloat3(&target);
	// （仮の）上方向
	XMVECTOR upVector = XMLoadFloat3(&up);

	// カメラZ軸（視線方向）
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
	// 0ベクトルだと向きが定まらないので除外
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));
	// ベクトルを正規化
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	// カメラのX軸（右方向）
	XMVECTOR cameraAxisX;
	// X軸は上方向→Z軸の外積で求まる
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	// ベクトルを正規化
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	// カメラのY軸（上方向）
	XMVECTOR cameraAxisY;
	// Y軸はZ軸→X軸の外積で求まる
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	// ここまでで直交した3方向のベクトルが揃う
	//（ワールド座標系でのカメラの右方向、上方向、前方向）	

	// カメラ回転行列
	XMMATRIX matCameraRot;
	// カメラ座標系→ワールド座標系の変換行列
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
	// 転置により逆行列（逆回転）を計算
	matView = XMMatrixTranspose(matCameraRot);

	// 視点座標に-1を掛けた座標
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	// カメラの位置からワールド原点へのベクトル（カメラ座標系）
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);	// X成分
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);	// Y成分
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);	// Z成分
	// 一つのベクトルにまとめる
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	// ビュー行列に平行移動成分を設定
	matView.r[3] = translation;

	// ビルボード行列
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
}

void Camera::UpdateProjectionMatrix()
{
	// 透視投影による射影行列の生成
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

void Camera::SetEye(XMFLOAT3 rotation, float distance)
{
	XMFLOAT3 newPos{};
	float newDist = distance;
	rot = rotation;

	newPos.x = target.x + newDist * cos(XMConvertToRadians(rot.y - 90)) * cos(XMConvertToRadians(rot.x));
	newPos.z = target.z + newDist * sin(XMConvertToRadians(rot.y - 90)) * cos(XMConvertToRadians(rot.x));

	newPos.y = target.y + 2 + newDist * sin(XMConvertToRadians(rot.x));

	Camera::eye = newPos;
	Camera::distance = newDist;

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
		//マウス操作
		rot.x += input->GetMouseCursorMovement().y * 0.025f;		//上下移動
		rot.y -= input->GetMouseCursorMovement().x * 0.025f;		//左右移動

		//キーボード操作
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

		if (rot.x < -70)
		{
			rot.x = -70;
		}
		if (rot.x > 70)
		{
			rot.x = 70;
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

	if (input->GetMouseWheelMovement() > 0 && distance > distMin)
	{
		distance -= distSpeed;
	}
	else if (input->GetMouseWheelMovement() < 0 && distance <= distMax)
	{
		distance += distSpeed;
	}

	CalcGroundCollisionPoint();
	CalcWallCollisionPoint();
}

void Camera::CalcGroundCollisionPoint()
{
	Ray ray{};
	ray.start = XMLoadFloat3(&target);
	ray.dir = XMVector3Normalize(eye - target);
	RaycastHit raycast{};

	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycast, distance))
	{
		XMVECTOR vec = raycast.inter - ray.dir;

		eye = {
			vec.m128_f32[0],
			vec.m128_f32[1],
			vec.m128_f32[2] };
	}
}

void Camera::CalcWallCollisionPoint()
{
	Ray ray{};
	ray.start = XMLoadFloat3(&eye);
	ray.dir = XMVector3Normalize(target - eye);
	RaycastHit raycast{};

	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_OBJECT_MESH, &raycast, distance))
	{
		return;
	}

	ray = {};
	ray.start = XMLoadFloat3(&target);
	ray.dir = XMVector3Normalize(eye - target);


	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_OBJECT_MESH, &raycast, distance))
	{
		XMVECTOR vec = raycast.inter - ray.dir;

		eye = {
			vec.m128_f32[0],
			vec.m128_f32[1],
			vec.m128_f32[2] };

	}
}
