#include "TouchableObject.h"
#include "MeshCollider.h"

TouchableObject* TouchableObject::Create(ModelOBJ* obj)
{
	//�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	TouchableObject* instance = new TouchableObject();
	if (instance == nullptr)
	{
		return nullptr;
	}
	//������
	if (!instance->Initialize(obj))
	{
		delete instance;
		assert(0);
	}

	return instance;
}

TouchableObject* TouchableObject::Create(ModelFBX* fbx)
{
	//�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	TouchableObject* instance = new TouchableObject();
	if (instance == nullptr)
	{
		return nullptr;
	}
	//������
	if (!instance->Initialize(fbx))
	{
		delete instance;
		assert(0);
	}

	return instance;
}

bool TouchableObject::Initialize(ModelOBJ* obj)
{
	if (!ObjectOBJ::Initialize() && !ObjectFBX::Initialize())
	{
		return false;
	}

	if (obj) 
	{
		SetModelOBJ(obj);

		//�R���C�_�[�̒ǉ�
		MeshCollider* collider = new MeshCollider;
		ObjectOBJ::SetCollider(collider);
		collider->ConstructTriangle(obj);
		collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	}

	return true;
}

bool TouchableObject::Initialize(ModelFBX* fbx)
{
	if (!ObjectOBJ::Initialize() && !ObjectFBX::Initialize())
	{
		return false;
	}

	if (fbx)
	{
		SetModelFBX(fbx);

		//�R���C�_�[�̒ǉ�
		MeshCollider* collider = new MeshCollider;
		ObjectFBX::SetCollider(collider);
		collider->ConstructTriangle(fbx);
		collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	}

	return true;
}

void TouchableObject::Update()
{
	if (ObjectOBJ::model)
	{
		ObjectOBJ::Update();
		ObjectOBJ::collider->Update();
	}
	if (ObjectFBX::model)
	{
		ObjectFBX::Update();
		ObjectFBX::collider->Update();
	}
}
