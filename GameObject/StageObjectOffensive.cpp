#include "StageObjectOffensive.h"
#include <CollisionAttribute.h>

StageObjectOffensive* StageObjectOffensive::Create(ModelFBX* fbx)
{
	// 3Dオブジェクトのインスタンスを生成
	StageObjectOffensive* instance = new StageObjectOffensive();
	if (instance == nullptr)
	{
		return nullptr;
	}

	// モデルのセット
	if (fbx)
	{
		instance->SetModelFBX(fbx);
	}

	// 初期化
	if (!instance->Initialize())
	{
		delete instance;
		assert(0);
	}

	return instance;
}

StageObjectOffensive* StageObjectOffensive::Create(ModelOBJ* obj)
{
	// 3Dオブジェクトのインスタンスを生成
	StageObjectOffensive* instance = new StageObjectOffensive();
	if (instance == nullptr)
	{
		return nullptr;
	}

	// モデルのセット
	if (obj)
	{
		instance->SetModelOBJ(obj);
	}

	// 初期化
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

	// コライダーの追加
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