#include "TouchableObject.h"
#include "MeshCollider.h"

TouchableObject* TouchableObject::Create(ModelOBJ* obj, ModelFBX* fbx)
{
	//オブジェクトのインスタンスを生成
	TouchableObject* instance = new TouchableObject();
	if (instance == nullptr)
	{
		return nullptr;
	}
	//初期化
	if (!instance->Initialize(obj, fbx))
	{
		delete instance;
		assert(0);
	}

	return instance;
}

bool TouchableObject::Initialize(ModelOBJ* obj, ModelFBX* fbx)
{
	if (!ObjectOBJ::Initialize() && !ObjectFBX::Initialize())
	{
		return false;
	}

	if (obj) 
	{
		SetModelOBJ(obj);

		//コライダーの追加
		MeshCollider* collider = new MeshCollider;
		ObjectOBJ::SetCollider(collider);
		collider->ConstructTriangle(obj);
		collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	}
	if (fbx)
	{
		SetModelFBX(fbx);

		//コライダーの追加
		MeshCollider* collider = new MeshCollider;
		ObjectFBX::SetCollider(collider);
		collider->ConstructTriangle(fbx);
		collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	}

	return true;
}
