#include "GameObject.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

GameObject* GameObject::Create(ModelFBX* fbx)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	GameObject* instance = new GameObject();
	if (instance == nullptr)
	{
		return nullptr;
	}

	// ������
	if (!instance->Initialize())
	{
		delete instance;
		assert(0);
	}

	// ���f���̃Z�b�g
	if (fbx)
	{
		instance->SetModelFBX(fbx);
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

	// ������
	if (!instance->Initialize())
	{
		delete instance;
		assert(0);
	}

	// ���f���̃Z�b�g
	if (obj)
	{
		instance->SetModelOBJ(obj);
	}

	return instance;
}

bool GameObject::Initialize()
{
	ObjectFBX::Initialize();
	ObjectOBJ::Initialize();

	// �R���C�_�[�̒ǉ�
	float radius = 0.6f;
	collider = new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius);

	if (ObjectOBJ::model) ObjectOBJ::SetCollider(collider);
	if (ObjectFBX::model) ObjectFBX::SetCollider(collider);
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);

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

	if (input->ReleaseMouse(MOUSE_LEFT))
	{
		isDrag = false;
		return;
	}

	XMVECTOR vec = input->CursorPoint3D(Camera::GetViewMatrix(), Camera::GetProjectionMatrix());

	if (input->TriggerMouse(MOUSE_LEFT))
	{
		// �擾�����x�N�g�������W�Ƃ��ĕۑ�
		pos = { vec.m128_f32[0], vec.m128_f32[1], vec.m128_f32[2] };

		// �擾�������W���w��͈͓��ɂȂ���Ί֐��𔲂���
		float deadZone = 10;
		if ((pos.x < ObjectOBJ::position.x - deadZone || pos.x > ObjectOBJ::position.x + deadZone) &&
			(pos.x < ObjectFBX::position.x - deadZone || pos.x > ObjectFBX::position.x + deadZone)) return;

		if ((pos.z < ObjectOBJ::position.z - deadZone || pos.z > ObjectOBJ::position.z + deadZone) &&
			(pos.z < ObjectFBX::position.z - deadZone || pos.z > ObjectFBX::position.z + deadZone)) return;

		// �͂�ł����Ԃɂ���
		isDrag = true;
	}
	// �͂܂�Ă��鎞�̏���
	if (isDrag)
	{
		pos = { vec.m128_f32[0], vec.m128_f32[1], vec.m128_f32[2] };
	}
}

void GameObject::OnCollision(const CollisionInfo& info)
{
}
