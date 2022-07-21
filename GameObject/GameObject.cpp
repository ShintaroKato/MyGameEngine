#include "GameObject.h"

GameObject* GameObject::Create(ModelFBX* fbx)
{
	// 3Dオブジェクトのインスタンスを生成
	GameObject* instance = new GameObject();
	if (instance == nullptr)
	{
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize())
	{
		delete instance;
		assert(0);
	}

	// モデルのセット
	if (fbx)
	{
		instance->SetModelFBX(fbx);
	}

	return instance;
}

GameObject* GameObject::Create(ModelOBJ* obj)
{
	// 3Dオブジェクトのインスタンスを生成
	GameObject* instance = new GameObject();
	if (instance == nullptr)
	{
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize())
	{
		delete instance;
		assert(0);
	}

	// モデルのセット
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

	// コライダーの追加
	/*float radius = 0.6f;
	collider = new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius);*/

	// 半径分だけ足元から浮いた座標を球の中心にする
	/*SetCollider(collider);
	collider->SetAttribute(COLLISION_ATTR_ALLIES);*/

	return true;
}

void GameObject::Update()
{
	Grab();
	Move();

	ObjectOBJ::Update();
	ObjectFBX::Update();
}

void GameObject::Move()
{
	// 掴まれていなければ関数を抜ける
	if (!isGrabbed) return;

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

void GameObject::Grab()
{
	Input* input = Input::GetInstance();

	if (input->ReleaseMouse(MOUSE_LEFT))
	{
		isGrabbed = false;
		return;
	}

	XMVECTOR vec = input->CursorPoint3D(Camera::GetViewMatrix(), Camera::GetProjectionMatrix());

	if (input->TriggerMouse(MOUSE_LEFT))
	{
		// 取得したベクトルを座標として保存
		pos = { vec.m128_f32[0], vec.m128_f32[1], vec.m128_f32[2] };

		// 取得した座標が指定範囲内になければ関数を抜ける
		float deadZone = 10;
		if ((pos.x < ObjectOBJ::position.x - deadZone || pos.x > ObjectOBJ::position.x + deadZone) &&
			(pos.x < ObjectFBX::position.x - deadZone || pos.x > ObjectFBX::position.x + deadZone)) return;

		if ((pos.z < ObjectOBJ::position.z - deadZone || pos.z > ObjectOBJ::position.z + deadZone) &&
			(pos.z < ObjectFBX::position.z - deadZone || pos.z > ObjectFBX::position.z + deadZone)) return;

		// 掴んでいる状態にする
		isGrabbed = true;
	}
	// 掴まれている時の処理
	if (isGrabbed)
	{
		pos = { vec.m128_f32[0], vec.m128_f32[1], vec.m128_f32[2] };
	}
}
