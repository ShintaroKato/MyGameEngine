#include "Player.h"
#include "Input.h"
#include "SphereCollider.h"
#include "Collision.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "ParticleEmitter.h"
#include "BulletManager.h"
#include "Math/Easing.h"

Player* Player::Create(ModelFBX* fbx, int animationNumber)
{
	// 3Dオブジェクトのインスタンスを生成
	Player* instance = new Player();
	if (instance == nullptr)
	{
		return nullptr;
	}

	// モデルのセット
	if (fbx)
	{
		instance->SetModelFBX(fbx);
		instance->SetAnimationNumber(animationNumber);
		instance->AnimationReset();
	}

	// 初期化
	if (!instance->Initialize())
	{
		delete instance;
		assert(0);
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

bool Player::Initialize()
{
	ObjectFBX::Initialize();
	ObjectOBJ::Initialize();

	// コライダーの追加
	sphere.center = XMLoadFloat3(&pos);
	sphere.center.m128_f32[1] += radius;
	sphere.radius = radius;
	sphereColl = new SphereCollider(sphere);

	sphereColl->SetAttribute(COLLISION_ATTR_ALLIES);
	if (ObjectOBJ::model) ObjectOBJ::SetCollider(sphereColl);
	if (ObjectFBX::model) ObjectFBX::SetCollider(sphereColl);

	return true;
}

void Player::Update()
{
	if (aliveFlag)
	{
		if (ObjectOBJ::model) pos = ObjectOBJ::position;
		if (ObjectFBX::model) pos = ObjectFBX::position;

		if (!isAnimated)
		{
			Move();
			Step();
			Jump();
			Attack();
		}

		SetPosition(pos);

		if (ObjectOBJ::model)
		{
			ObjectOBJ::rotation = rot;
			ObjectOBJ::UpdateWorldMatrix();
			ObjectOBJ::collider->Update();
		}
		if (ObjectFBX::model)
		{
			ObjectFBX::rotation = rot;
			ObjectFBX::UpdateWorldMatrix();
			ObjectFBX::collider->Update();
		}

		Rejection();

		SetColor({ 1,1,1,GetCameraDistance() / 2 });

		ObjectOBJ::Update();
		ObjectFBX::Update();
	}
}

void Player::Move()
{
	if (stepTimer < stepEnd) return;

	Input* input = Input::GetInstance();

	//向いている方向に移動
	if (input->PushKey(DIK_S) || input->PushKey(DIK_W) ||
		input->PushKey(DIK_D) || input->PushKey(DIK_A) )
	{
		if (input->PushKey(DIK_W))
		{
			rot.y = 0;
		}
		if (input->PushKey(DIK_D))
		{
			rot.y = 90;
		}
		if (input->PushKey(DIK_S))
		{
			rot.y = 180;
		}
		if (input->PushKey(DIK_A))
		{
			rot.y = 270;
		}
		if (input->PushKey(DIK_W) && input->PushKey(DIK_D))
		{
			rot.y = 45;
		}
		if (input->PushKey(DIK_D) && input->PushKey(DIK_S))
		{
			rot.y = 135;
		}
		if (input->PushKey(DIK_S) && input->PushKey(DIK_A))
		{
			rot.y = 225;
		}
		if (input->PushKey(DIK_A) && input->PushKey(DIK_W))
		{
			rot.y = 315;
		}

		float cameraRotY = -camera->GetRotation().y;
		rot.y += cameraRotY;

		//移動ベクトルをy軸周りの角度で回転
		move = move_default;
		XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rot.y));
		move = XMVector3TransformNormal(move, matRot);

		pos.x += move.m128_f32[0];
		pos.y += move.m128_f32[1];
		pos.z += move.m128_f32[2];
	}
}

void Player::Jump()
{
	if (attackFlag) return;

	if(stepTimer > stepEnd)
	{
		// 落下処理
		if (jumpState != ON_GROUND)
		{
			// 下向き加速度
			const float fallAcc = -0.01f;
			const float fallVYMin = -0.5f;
			// 加速
			fallVel.m128_f32[1] = max(fallVel.m128_f32[1] + fallAcc, fallVYMin);
			// 移動
			pos.x += fallVel.m128_f32[0];
			pos.y += fallVel.m128_f32[1];
			pos.z += fallVel.m128_f32[2];

			if(jumpState != WALL_JUMP_AFTER)
			{
				jumpState = STAY_IN_AIR;
			}
			if (jumpState == STAY_IN_AIR && hitWall)
			{
				jumpState = WALL_JUMP_BEFORE;
			}
		}
		else jumpState = ON_GROUND;

		// ジャンプ操作
		if (Input::GetInstance()->TriggerKey(DIK_SPACE) &&
			(jumpState != STAY_IN_AIR && jumpState != WALL_JUMP_AFTER))
		{
			const float jumpVYFist = 0.3f;
			fallVel = { 0, jumpVYFist, 0, 0 };

			if(jumpState == WALL_JUMP_BEFORE)
			{
				jumpState = WALL_JUMP_AFTER;
			}
			else if(jumpState != WALL_JUMP_AFTER)
			{
				jumpState = STAY_IN_AIR;
			}

			ParticleEmitter::CircleXZ(64, 8, pos, 0.0f, 0.2f,-0.01f,
				{ 0.5f,0.5f,1.0f,1.0f }, { 0.0f,0.0f,1.0f,1.0f },
				1.0f, 0.5f);
		}
	}

	// 球の上端から球の下端までのレイキャスト
	Ray ray;
	ray.start = XMLoadFloat3(&pos);
	ray.start.m128_f32[1] += sphereColl->GetRadius() * 2.0f;
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;

	// 接地状態
	if (jumpState == ON_GROUND)
	{
		// スムーズに坂を下る為の吸着距離
		const float adsDistance = 0.2f;
		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereColl->GetRadius() * 2.0f + adsDistance) ||
			CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_OBJECT_MESH, &raycastHit, sphereColl->GetRadius() * 2.0f + adsDistance))
		{
			jumpState == ON_GROUND;
			sphereColl->center.m128_f32[1] = raycastHit.distance - sphereColl->GetRadius();
			pos.y -= sphereColl->center.m128_f32[1] - sphereColl->GetRadius();
		}
		// 地面がないので落下
		else
		{
			jumpState = STAY_IN_AIR;
			fallVel = {};
		}
	}
	// 落下状態
	else if (fallVel.m128_f32[1] <= 0.0f)
	{
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereColl->GetRadius() * 2.0f) ||
			CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_OBJECT_MESH, &raycastHit, sphereColl->GetRadius() * 2.0f))
		{
			// 着地
			jumpState = ON_GROUND;
			sphereColl->center.m128_f32[1] = raycastHit.distance - sphereColl->GetRadius();
			pos.y -= sphereColl->center.m128_f32[1] - sphereColl->GetRadius();
		}
	}
}

void Player::Step()
{
	Input* input = Input::GetInstance();

	if (input->TriggerKey(DIK_LSHIFT) && !isStepped)
	{
		jumpState = STAY_IN_AIR;

		stepTimer = 0;
		isStepped = true;

		// ステップ開始地点
		stepStartPos = pos;
		stepEndPos = pos;
	}
	if (isStepped)
	{
		if (stepTimer == 0)
		{
			for (int i = 0; i <= stepEnd; i++)
			{
				stepEndPos.x += move.m128_f32[0] * 2;
				stepEndPos.y += move.m128_f32[1] * 2;
				stepEndPos.z += move.m128_f32[2] * 2;
			}

			Ray ray;
			ray.start = XMLoadFloat3(&stepStartPos);
			ray.start.m128_f32[1] += radius;
			ray.dir = move;
			RaycastHit raycastHit;

			float lengthStoE = sqrtf(
				(stepEndPos.x - stepStartPos.x) * (stepEndPos.x - stepStartPos.x) +
				(stepEndPos.y - stepStartPos.y) * (stepEndPos.y - stepStartPos.y) +
				(stepEndPos.z - stepStartPos.z) * (stepEndPos.z - stepStartPos.z));

			if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_OBJECT_MESH, &raycastHit, lengthStoE))
			{
				stepEndPos.x = raycastHit.inter.m128_f32[0];
				stepEndPos.y = raycastHit.inter.m128_f32[1];
				stepEndPos.z = raycastHit.inter.m128_f32[2];

				ParticleEmitter::LaserBeam(10, 10, pos, stepEndPos, 2.0f,
					{ 1,1,1,1 }, { 1,1,1,1 }, 0, 0, 0.5f, 0.5f);

				ParticleEmitter::EmitRandomAllRange(4, 4, stepEndPos, { 0,0,0 },
					{ 1.0f,1.0f,0.8f,1.0f }, { 1.0f,1.0f,0.0f,1.0f }, 0, 0, 1.0f, 2.0f);
			}
		}
		if (stepTimer < stepTimerMax)
		{
			stepTimer++;

			if (stepTimer < stepEnd)
			{
				XMFLOAT3 ease = Easing::EaseIn(stepTimer, stepStartPos, stepEndPos);

				if (hitWall) 
				{
  					stepTimer = stepEnd;
				}
				else pos = ease;
			}
		}
		else isStepped = false;
	}
}

void Player::Rejection()
{

	// クエリコールバッククラス
	class PlayerQueryCallback : public QueryCallback
	{
	public:
		PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

		bool OnQueryHit(const QueryHit& info)
		{
			// ワールドの上方向
			const XMVECTOR up = { 0.1,0,0 };
			// 排斥方向
			XMVECTOR rejectDir = XMVector3Normalize(info.reject);
			// 上方向と排斥ベクトルのコサイン値
			float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

			// 地面判定しきい値角度
			const float threshold = cosf(XMConvertToRadians(30.0f));
			// 角度差によって天井または地面と判定される場合を除いて
			if (-threshold < cos && cos < threshold)
			{
				// 球を排斥
				sphere->center += info.reject;
				move += info.reject;
				hit = true;
			}
			else
			{
				hit = false;
			}


			return hit;
		}

		// クエリに使用する球
		Sphere* sphere = nullptr;
		// 排斥による移動量(累積値)
		DirectX::XMVECTOR move = {};
		// 衝突していたらtrue
		bool hit = false;
	};

	// クエリコールバックの関数オブジェクト
	PlayerQueryCallback callback(sphereColl);

	// 球と地形の交差を全検索
	CollisionManager::GetInstance()->QuerySphere(*sphereColl, &callback, COLLISION_ATTR_OBJECT_MESH);
	// 交差による排斥分動かす
	pos.x += callback.move.m128_f32[0];
	pos.y += callback.move.m128_f32[1];
	pos.z += callback.move.m128_f32[2];

	if (weapon)
	{
		XMFLOAT3 pos = weapon->GetPosition();
		pos.x += callback.move.m128_f32[0];
		pos.y += callback.move.m128_f32[1];
		pos.z += callback.move.m128_f32[2];
		weapon->SetPosition(pos);
		weapon->Update();
	}

	hitWall = callback.hit;

	// コライダー更新
	SetPosition(pos);

	if (ObjectOBJ::model)
	{
		ObjectOBJ::rotation = rot;
		ObjectOBJ::UpdateWorldMatrix();
		ObjectOBJ::collider->Update();
	}
	if (ObjectFBX::model)
	{
		ObjectFBX::rotation = rot;
		ObjectFBX::UpdateWorldMatrix();
		ObjectFBX::collider->Update();
	}
}

void Player::Attack()
{
	Input* input = Input::GetInstance();

	// 攻撃
	if (input->TriggerButton(Button_A_Cross) || input->TriggerMouse(MOUSE_LEFT))
	{
		if (attackLevel < 3 && attackCount < 8.0f && weapon)
		{
			weapon->SetPosition({
				pos.x + sin(XMConvertToRadians(rot.y)) * 1.5f, pos.y + 1,
				pos.z + cos(XMConvertToRadians(rot.y)) * 1.5f });

			attackLevel++;
			attackCount = 16.0f;
			attackFlag = true;

			BulletManager::GetInstance()->Fire(pos, rot, COLLISION_ATTR_ALLIES, 1.0f, power, false);
		}
	}

	if (attackFlag) {

		attackCount -= 1.0f;

		if (attackCount < 14.0f && attackCount > -4.0f)
		{
			pos.x += move.m128_f32[0] * attackMove;
			pos.z += move.m128_f32[2] * attackMove;
		}

		// カウント0で攻撃中フラグをfalseにする
		if (attackCount < -10)
		{
			attackFlag = false;
		}

		if (weapon)
		{
			XMFLOAT3 wPos = { pos.x + sin(XMConvertToRadians(rot.y - 90 + (attackCount * attackCount - attackCount))),
				pos.y + 0.3f,
				pos.z + cos(XMConvertToRadians(rot.y - 90 + (attackCount * attackCount - attackCount))) };

			XMFLOAT3 wRot = { rot.x - 90,
				rot.y + (attackCount * attackCount - attackCount),
				rot.z + 90 };

			weapon->SetPosition(wPos);
			weapon->SetRotation(wRot);
			weapon->Update();
		}

		if ((int)attackCount % 5 == 0)
		{
			CollisionManager::GetInstance()->CheckAllCollision(weapon->GetSphereCollider(), COLLISION_ATTR_ENEMIES);
		}

		ParticleEmitter::EmitRandomAllRange(5, attackCount, weapon->GetPosition(),
			{ 0,0,0 },
			{ 0.6f,0.6f,1.0f,1.0f }, { 0.0f,0.0f,1.0f,1.0f }, 0.1f, 0.01f, 0.5f);
	}
	else
	{
		attackLevel = 0;

		if (weapon)
		{
			XMFLOAT3 wPos = { pos.x + sin(XMConvertToRadians(rot.y + 90)),
				pos.y + 0.2f,
				pos.z + cos(XMConvertToRadians(rot.y + 90)) };
			XMFLOAT3 wRot = { rot.x,rot.y,rot.z - 90 };

			weapon->SetPosition(wPos);
			weapon->SetRotation(wRot);
			weapon->Update();
		}
	}
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

void Player::OnCollision(const CollisionInfo& info)
{
	if (info.collider->GetAttribute() == COLLISION_ATTR_OBJECT_MESH)
	{
	}
}

void Player::SetPosition(XMFLOAT3 pos)
{
	// コライダーの追加
	sphere.center = XMLoadFloat3(&pos);
	sphere.center.m128_f32[1] += radius;
	sphere.radius = radius;

	sphereColl->SetSphere(sphere);

	if (ObjectOBJ::model)
	{
		ObjectOBJ::SetPosition(pos);
	}
	if (ObjectFBX::model)
	{
		ObjectFBX::SetPosition(pos);
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

void Player::SetWeapon(Weapon* weapon)
{
	this->weapon = weapon;

	if (ObjectOBJ::model)
	{
		this->weapon->SetParent((ObjectOBJ*)this);
	}
	if(ObjectFBX::model)
	{
		this->weapon->SetParent((ObjectFBX*)this);
	}

	this->weapon->SetPosition(pos);
	this->sphereColl->SetPower(GetPower());
}

bool Player::SpawnAnimation()
{
	float rate = animTimer / animTimerMax;

	if (animTimer < animTimerMax)
	{
		animTimer++;

		XMFLOAT3 effectPos = pos;
		effectPos.y += radius / 2;
		// 周囲に輪
		ParticleEmitter::CircleXZ(64, 8, effectPos, 80.0f, 0.2f, -0.01f,
			{ 0.5f,0.5f,1.0f,1.0f }, { 0.0f,0.0f,1.0f,1.0f },
			0.5f / (animTimer / 10), 0.1f / (animTimer / 10));
		// 光を放つ感じ
		ParticleEmitter::EmitRandomAllRange(5, attackCount, effectPos,
			{ 0,0,0 },
			{ 0.6f,0.6f,1.0f,1.0f }, { 0.0f,0.0f,1.0f,1.0f }, 0.1f, 0.01f,
			0.5f, 0.2f);


		XMFLOAT3 effectPosHead = pos;
		XMFLOAT3 effectPosTail = pos;
		effectPosHead.y += 10;
		effectPosTail.y -= 10;
		ParticleEmitter::LaserBeam(16, 16, effectPosTail, effectPosHead, 0.1f,
			{ 0.2f,0.2f,1.0f,0.01f }, { 0.2f,0.2f,1.0f,0.01f }, 0.1f, 0.0f,
			(animTimerMax / 10) / (animTimer + 0.01), (animTimerMax / 10) / (animTimer + 0.01));

		isAnimated = true;
	}
	else
	{
		isAnimated = false;
	}

	SetColor({ rate,rate,1.0f,1.0f });
	ObjectOBJ::Update();

	return isAnimated;
}
