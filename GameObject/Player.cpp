#include "Player.h"

Player* Player::Create(ModelFBX* model)
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
	if (model)
	{
		instance->SetModel(model);
	}

	return instance;
}

bool Player::Initialize()
{
	if (!ObjectFBX::Initialize())
	{
		return false;
	}

	// コライダーの追加
	/*float radius = 0.6f;
	collider = new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius);*/

	// 半径分だけ足元から浮いた座標を球の中心にする
	/*SetCollider(collider);
	collider->SetAttribute(COLLISION_ATTR_ALLIES);*/

	return true;
}

void Player::Update()
{
	Move();
	Attack();
}

void Player::Move()
{
	
}

void Player::Attack()
{
}

bool Player::Hit()
{
	return false;
}
