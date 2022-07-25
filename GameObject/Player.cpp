#include "Player.h"
#include "Input.h"

Player* Player::Create(ModelFBX* fbx, int animationNumber)
{
	// 3Dオブジェクトのインスタンスを生成
	Player* instance = new Player();
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
		instance->SetAnimationNumber(animationNumber);
		instance->AnimationReset();
	}

	return instance;
}

Player* Player::Create(ModelOBJ* obj)
{
	// 3Dオブジェクトのインスタンスを生成
	Player* instance = new Player();
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

bool Player::Initialize()
{
	ObjectFBX::Initialize();
	ObjectOBJ::Initialize();

	// コライダーの追加
	float radius = 0.6f;
	collider = new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius);

	// 半径分だけ足元から浮いた座標を球の中心にする
	SetCollider(collider);
	collider->SetAttribute(COLLISION_ATTR_ALLIES);

	return true;
}

void Player::Update()
{
	Move();
	Attack();

	this->ObjectOBJ::Update();
	this->ObjectFBX::Update();
}

void Player::Move()
{
	Input* input = new Input();

	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) ||
		input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{
		if (input->PushKey(DIK_UP))
		{
		}
		else if (input->PushKey(DIK_DOWN))
		{
		}

		if (input->PushKey(DIK_RIGHT))
		{
		}
		else if (input->PushKey(DIK_LEFT))
		{
		}
	}

}

void Player::Attack()
{
}

bool Player::Hit()
{
	return false;
}

XMFLOAT3 Player::GetPosition()
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

void Player::SetPosition(XMFLOAT3 pos)
{
	if (ObjectOBJ::model)
	{
		ObjectOBJ::position = pos;
	}
	if (ObjectFBX::model)
	{
		ObjectFBX::position = pos;
	}
}

void Player::SetRotation(XMFLOAT3 rot)
{
	if (ObjectOBJ::model)
	{
		ObjectOBJ::rotation = rot;
	}
	if (ObjectFBX::model)
	{
		ObjectFBX::rotation = rot;
	}
}

void Player::SetScale(XMFLOAT3 scale)
{
	if (ObjectOBJ::model)
	{
		ObjectOBJ::scale = scale;
	}
	if (ObjectFBX::model)
	{
		ObjectFBX::scale = scale;
	}
}
