#include "StageObjectOffensive.h"
#include <CollisionAttribute.h>

StageObjectOffensive* StageObjectOffensive::Create(ModelFBX* fbx)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	StageObjectOffensive* instance = new StageObjectOffensive();
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

StageObjectOffensive* StageObjectOffensive::Create(ModelOBJ* obj)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	StageObjectOffensive* instance = new StageObjectOffensive();
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

bool StageObjectOffensive::Initialize()
{
	ObjectFBX::Initialize();
	ObjectOBJ::Initialize();

	// �R���C�_�[�̒ǉ�
	sphere.center = { pos.x, pos.y + radius, pos.z,0 };
	sphere.radius = radius;

	meshColl = new MeshCollider();

	SetPosition(pos);

	return true;
}

void StageObjectOffensive::Update()
{
	if (HP <= 0) aliveFlag = false;

	StageObject::Update();
}

void StageObjectOffensive::Draw()
{
	if (!aliveFlag) return;

	ObjectOBJ::Draw();
}