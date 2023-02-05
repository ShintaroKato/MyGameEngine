#include "PlaneCursor.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "Collision.h"
#include "Input.h"
#include "SceneManager.h"

ObjectOBJ* PlaneCursor::cursor;
XMFLOAT3 PlaneCursor::pos;
float PlaneCursor::radius = 5.0f;
bool PlaneCursor::isDrag;
bool PlaneCursor::used;
bool PlaneCursor::isInGame;
Sphere PlaneCursor::sphere;
SphereCollider* PlaneCursor::sphereColl;

void PlaneCursor::Initialize(ObjectOBJ* object)
{
	cursor = object;

	// コライダーの追加
	sphere.center = XMLoadFloat3(&pos);
	sphere.center.m128_f32[1] += radius;
	sphere.radius = radius;

	sphereColl = new SphereCollider(sphere, true);

	SetPosition(cursor->GetPosition());
}

void PlaneCursor::Update()
{
	if (SceneManager::GetScene() != EDIT) return;

	Move();

	SetPosition(pos);

	cursor->Update();
}

void PlaneCursor::Move()
{
	Input* input = Input::GetInstance();

	XMVECTOR vec = input->CursorPoint3D(Camera::GetViewMatrix(), Camera::GetProjectionMatrix());
	Ray ray;
	ray.start = input->CalcScreenToWorld(input->GetMousePos2(), 0.0f, Camera::GetViewMatrix(), Camera::GetProjectionMatrix());
	ray.dir = input->CalcScreenToWorld(input->GetMousePos2(), 1.0f, Camera::GetViewMatrix(), Camera::GetProjectionMatrix());
	ray.dir = XMVector3Normalize(ray.dir - ray.start);
	float distance = 0;
	XMVECTOR inter{};

	pos = { vec.m128_f32[0], 0, vec.m128_f32[2] };
	sphere.center = XMLoadFloat3(&pos);
	sphereColl->SetOffset(sphere.center);

	// 動かせる範囲
	if (pos.x < -60) pos.x = -60;
	if (pos.x > 60) pos.x = 60;

	if (pos.z < -60) pos.z = -60;
	if (pos.z > 60) pos.z = 60;

	if (pos.y > 0) pos.y = 0;
}

void PlaneCursor::Draw()
{
	if (SceneManager::GetScene() != EDIT) return;

	cursor->Draw();
}

void PlaneCursor::SetPosition(const XMFLOAT3& position)
{
	pos = position;
	pos.y = 0.0f;

	// コライダーの追加
	sphere.center = XMLoadFloat3(&pos);
	sphere.center.m128_f32[1] += radius;
	sphere.radius = radius;

	*sphereColl = SphereCollider(sphere, true);
	sphereColl->SetAttribute(COLLISION_ATTR_PLANE_CURSOR);

	cursor->SetScale({
		radius * 2, 1.0f,
		radius * 2 });
	cursor->SetPosition(pos);
	cursor->SetCollider(sphereColl);
}