#include "GameObject.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "Collision.h"

GameObject* GameObject::Create(ModelFBX* fbx)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	GameObject* instance = new GameObject();
	if (instance == nullptr)
	{
		return nullptr;
	}

	// ���f���̃Z�b�g
	if (fbx)
	{
		instance->SetModelFBX(fbx);
	}

	// ������
	if (!instance->Initialize())
	{
		delete instance;
		assert(0);
	}

	return instance;
}

GameObject* GameObject::Create(ModelOBJ* obj)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	GameObject* instance = new GameObject();
	if (instance == nullptr)
	{
		return nullptr;
	}

	// ���f���̃Z�b�g
	if (obj)
	{
		instance->SetModelOBJ(obj);
	}

	// ������
	if (!instance->Initialize())
	{
		delete instance;
		assert(0);
	}

	return instance;
}

bool GameObject::Initialize()
{
	ObjectFBX::Initialize();
	ObjectOBJ::Initialize();

	// �R���C�_�[�̒ǉ�
	sphere.center = { pos.x, pos.y + radius, pos.z,0 };
	sphere.radius = radius;
	sphereColl = new SphereCollider(sphere);

	if (ObjectOBJ::model) ObjectOBJ::SetCollider(sphereColl);
	if (ObjectFBX::model) ObjectFBX::SetCollider(sphereColl);
	sphereColl->SetAttribute(COLLISION_ATTR_OBJECT);

	return true;
}

void GameObject::Update()
{
	Drag();
	Move();

	ObjectOBJ::Update();
	ObjectFBX::Update();
}

void GameObject::Move()
{
	// �͂܂�Ă��Ȃ���Ί֐��𔲂���
	if (!isDrag) return;

	if (ObjectOBJ::model != nullptr)
	{
		ObjectOBJ::position = pos;
	}
	else if (ObjectFBX::model != nullptr)
	{
		ObjectFBX::position = pos;
	}
}

bool GameObject::Hit()
{
	return false;
}

void GameObject::Drag()
{
	if(fix) return;

	Input* input = Input::GetInstance();

	if (!input->PushMouse(MOUSE_LEFT))
	{
		isDrag = false;
		return;
	}

	XMVECTOR vec = input->CursorPoint3D(Camera::GetViewMatrix(), Camera::GetProjectionMatrix());
	Ray ray;
	ray.start = input->CalcScreenToWorld(input->GetMousePos2(), 0.0f, Camera::GetViewMatrix(), Camera::GetProjectionMatrix());
	ray.dir = input->CalcScreenToWorld(input->GetMousePos2(), 1.0f, Camera::GetViewMatrix(), Camera::GetProjectionMatrix());
	ray.dir = XMVector3Normalize(ray.dir - ray.start);
	float distance = 0;
	XMVECTOR inter{};

	if (input->TriggerMouse(MOUSE_LEFT))
	{
		if (Collision::ChackRay2Sphere(ray, sphere, &distance, &inter))
		{
			// �͂�ł����Ԃɂ���
			isDrag = true;
		}
	}
	// �͂܂�Ă��鎞�̏���
	if (isDrag)
	{
		pos = { vec.m128_f32[0], vec.m128_f32[1], vec.m128_f32[2] };
		SetPosition(pos);
	}
}

void GameObject::SetPosition(const XMFLOAT3& pos)
{
	// �R���C�_�[�̒ǉ�
	sphere.center = { pos.x, pos.y + radius, pos.z,0 };
	sphere.radius = radius;

	sphereColl->SetSphere(sphere);

	if (ObjectOBJ::model)
	{
		ObjectOBJ::SetPosition(pos);
		ObjectOBJ::SetCollider(sphereColl);
	}
	if (ObjectFBX::model)
	{
		ObjectFBX::SetPosition(pos);
		ObjectFBX::SetCollider(sphereColl);
	}
}

XMFLOAT3 GameObject::GetPosition()
{
	if (ObjectOBJ::model)
	{
		return ObjectOBJ::position;
	}
	if (ObjectFBX::model)
	{
		return ObjectFBX::position;
	}
}

void GameObject::OnCollision(const CollisionInfo& info)
{
}
