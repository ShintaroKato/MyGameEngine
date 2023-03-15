#include "Enemy.h"
#include "Input.h"
#include "SphereCollider.h"
#include "Collision.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "ParticleEmitter.h"
#include "BulletManager.h"

Enemy* Enemy::Create(ModelFBX* fbx, int animationNumber)
{
	// 3Dオブジェクトのインスタンスを生成
	Enemy* instance = new Enemy();
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

Enemy* Enemy::Create(ModelOBJ* obj)
{
	// 3Dオブジェクトのインスタンスを生成
	Enemy* instance = new Enemy();
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

bool Enemy::Initialize()
{
	ObjectFBX::Initialize();
	ObjectOBJ::Initialize();
	// コライダーの追加
	sphere.center = XMLoadFloat3(&pos);
	sphere.center.m128_f32[1] += radius;
	sphere.radius = radius;
	sphereColl = new SphereCollider(sphere);

	if (ObjectOBJ::model) ObjectOBJ::SetCollider(sphereColl);
	if (ObjectFBX::model) ObjectFBX::SetCollider(sphereColl);

	return true;
}

void Enemy::Update()
{
	if (!aliveFlag) return;

	if (ObjectOBJ::model) pos = ObjectOBJ::position;
	if (ObjectFBX::model) pos = ObjectFBX::position;


	switch (type)
	{
	case FLYING:
		Fly();
		targetPos = target->GetPosition();
		AttackBeam();
		break;

	case ROUTE_SEARCH:
		MoveRouteSearch();
		Attack();
		break;

	case STRAIGHT:
	default:
		Move();
		// この位置で一旦、城オブジェクトの座標を標的として設定
		// Attack()で標的の座標がかわる
		targetPos = target->GetPosition();
		// 攻撃
		Attack();
		break;
	}

	// 無敵時間カウント
	if (noDamageTime > 0)	 noDamageTime--;
	else					 noDamageTime = 0;
	Defeated();


	SetPosition(pos);

	if (ObjectOBJ::model)
	{
		ObjectOBJ::rotation = rot;
		ObjectOBJ::collider->Update();
	}
	if (ObjectFBX::model)
	{
		ObjectFBX::rotation = rot;
		ObjectFBX::collider->Update();
	}
	ObjectOBJ::Update();
	ObjectFBX::Update();
}

void Enemy::Draw()
{
	if (!aliveFlag || noDamageTime % 2 == 1) return;

	if(ObjectOBJ::model) ObjectOBJ::Draw();
	//if(ObjectFBX::model) ObjectFBX::Draw();
}

void Enemy::Move()
{
	// 標的の方向を向く
	rot.y = XMConvertToDegrees(
		atan2f(targetPos.x - pos.x, targetPos.z - pos.z));

	move = { 0,0,0.1f,0 };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rot.y));
	move = XMVector3TransformNormal(move, matRot);

	float nearLimit = 9.0f;

	//ある程度の距離を保ち続ける処理
	if (abs(pos.x - targetPos.x) < nearLimit && abs(pos.z - targetPos.z) < nearLimit)
	{
		isStop = true;
	}
	else
	{
		isStop = false;
		pos.x += move.m128_f32[0];
		pos.y += move.m128_f32[1];
		pos.z += move.m128_f32[2];
	}
}

void Enemy::MoveRouteSearch()
{
	move = moveDefault;
	XMMATRIX matRot{};

	XMVECTOR searchVec{};
	Ray searchRay;
	RaycastHit hit;
	bool rayHit = false;
	bool rayHitR = false;
	bool rayHitL = false;
	float rayDist = 10.0f;

	float rotY = XMConvertToDegrees(
			atan2f(targetPos.x - pos.x, targetPos.z - pos.z));
	matRot = XMMatrixRotationY(XMConvertToRadians(rotY));
	searchVec = XMVector3TransformNormal(move, matRot);
	searchRay.start = XMLoadFloat3(&pos);
	searchRay.dir = searchVec;

	// 現在地から標的までの間に障害物があるかを判定
	rayHit = CollisionManager::GetInstance()->Raycast(searchRay, COLLISION_ATTR_OBJECT_MESH, &hit);
	if (rayHit == false)
	{
		// 標的の方向を向く
		rot.y = rotY;
	}
	else
	{
		rayDist = 6.0f;

#pragma region 右側

		matRot = XMMatrixRotationY(XMConvertToRadians(rot.y + 30)); // 進行方向から30度ずらす

		searchVec = XMVector3TransformNormal(move, matRot);
		searchRay.start = XMLoadFloat3(&pos);
		searchRay.dir = searchVec;
		rayHitR = CollisionManager::GetInstance()->Raycast(searchRay, COLLISION_ATTR_OBJECT_MESH, &hit);
		if (rayHitR == true)
		{
			if (hit.distance <= rayDist && hit.collider != target->GetCollider()) rot.y -= 4.0f;
		}
		else
		{
			matRot = XMMatrixRotationY(XMConvertToRadians(rot.y + 60));// 進行方向から60度ずらす

			searchVec = XMVector3TransformNormal(move, matRot);
			searchRay.start = XMLoadFloat3(&pos);
			searchRay.dir = searchVec;
			rayHitR = CollisionManager::GetInstance()->Raycast(searchRay, COLLISION_ATTR_OBJECT_MESH, &hit);
			if (rayHitR == true)
			{
				// 方向を変えない
				if (hit.distance <= rayDist && hit.collider != target->GetCollider()) rot.y = rot.y;
				else if (hit.collider == target->GetCollider()) rot.y = rotY;
				else rayHitR = false;
			}
		}

#pragma endregion

#pragma region 左側

		matRot = XMMatrixRotationY(XMConvertToRadians(rot.y - 30)); // 進行方向から-30度ずらす

		searchVec = XMVector3TransformNormal(move, matRot);
		searchRay.start = XMLoadFloat3(&pos);
		searchRay.dir = searchVec;
		rayHitL = CollisionManager::GetInstance()->Raycast(searchRay, COLLISION_ATTR_OBJECT_MESH, &hit);
		if (rayHitR == false && rot.y != rotY)
		{
			if (rayHitL == true)
			{
				if (hit.distance <= rayDist && hit.collider != target->GetCollider()) rot.y += 8.0f;
			}
			else
			{
				matRot = XMMatrixRotationY(XMConvertToRadians(rot.y - 60));// 進行方向から-60度ずらす

				searchVec = XMVector3TransformNormal(move, matRot);
				searchRay.start = XMLoadFloat3(&pos);
				searchRay.dir = searchVec;
				rayHitL = CollisionManager::GetInstance()->Raycast(searchRay, COLLISION_ATTR_OBJECT_MESH, &hit);
				if (rayHitL == true)
				{
					if (hit.distance <= rayDist && hit.collider != target->GetCollider()) rot.y = rot.y;// 方向を変えない
					else if (hit.collider == target->GetCollider()) rot.y = rotY;

					else rayHitL = false;
				}
			}
		}

#pragma endregion
	}


	matRot = XMMatrixRotationY(XMConvertToRadians(rot.y));
	move = XMVector3TransformNormal(move, matRot);

	float nearLimit = 9.0f;

	//ある程度の距離を保ち続ける処理
	if (abs(pos.x - targetPos.x) < nearLimit && abs(pos.z - targetPos.z) < nearLimit)
	{
		isStop = true;
	}
	else
	{
		isStop = false;
		pos.x += move.m128_f32[0];
		pos.y += move.m128_f32[1];
		pos.z += move.m128_f32[2];
	}
}

void Enemy::Fly()
{
	// 常に少し浮かせる
	pos.y = 25.0f;
	// 標的の方向を向く
	rot.y = XMConvertToDegrees(
		atan2f(targetPos.x - pos.x, targetPos.z - pos.z));

	move = { 0,0,0.1f,0 };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rot.y));
	move = XMVector3TransformNormal(move, matRot);

	float nearLimit = 30.0f;

	//ある程度の距離を保ち続ける処理
	if (abs(pos.x - targetPos.x) < nearLimit && abs(pos.z - targetPos.z) < nearLimit)
	{
		isStop = true;
	}
	else
	{
		isStop = false;
		pos.x += move.m128_f32[0];
		pos.y += move.m128_f32[1];
		pos.z += move.m128_f32[2];
	}

	XMFLOAT3 effect = pos;
	effect.x = effect.x - 6 * sin(XMConvertToRadians(rot.y));
	effect.z = effect.z - 6 * cos(XMConvertToRadians(rot.y));
	ParticleEmitter::EmitZ_AxisDir(4, 8, effect, 0,
		{ 1.0f,0.0f,0.0f,1.0f }, { 1.0f,0.5f,0.5f,1.0f },
		0.01f, 0.001f, 8.0f, 1.0f
	);
}

void Enemy::Attack()
{
	sphereColl->SetPower(attackPower);
	CollisionManager::GetInstance()->CheckAllCollision(sphereColl, COLLISION_ATTR_OBJECT_MESH);
}

void Enemy::AttackBeam()
{
	if (!isStop) return;

	XMFLOAT3 target = targetPos;
	XMFLOAT3 cannonPos = pos;

	if (attackCount > 0)
	{
		// チャージエフェクト
		XMFLOAT3 effect = pos;
		effect.x = effect.x + 4 * sin(XMConvertToRadians(rot.y));
		effect.z = effect.z + 4 * cos(XMConvertToRadians(rot.y));
		effect.y -= 3;
		ParticleEmitter::EmitZ_AxisDir(4, 8, effect, 0,
			{ 1.0f,0.0f,0.0f,1.0f }, { 1.0f,0.5f,0.5f,1.0f },
			0.1f, 0.001f, 10.0f / (attackCount / 2), 1.0f
		);
	}
	if (attackCount <= 0)
	{

		XMFLOAT3 dist{};
		dist.x = target.x - cannonPos.x;
		dist.y = target.y - cannonPos.y;
		dist.z = target.z - cannonPos.z;

		XMFLOAT3 cannonRot{};
		cannonRot.y = XMConvertToDegrees(atan2f(dist.x, dist.z));
		cannonRot.x = -XMConvertToDegrees(atan2f(dist.y, sqrtf(dist.x * dist.x + dist.z * dist.z)));

		if(attackCount % 10 == 0)
		{
			BulletManager::GetInstance()->Fire(cannonPos, cannonRot, COLLISION_ATTR_ENEMIES, 3.0f, attackPower);
		}

		Ray ray;
		ray.start = XMLoadFloat3(&cannonPos);
		ray.dir = XMLoadFloat3(&target);
		ray.dir = XMVector3Normalize(ray.dir - ray.start);
		RaycastHit raycast{};
		XMFLOAT3 inter{};

		CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_ENEMIES, sphereColl, &raycast);

		XMFLOAT3 ePos{};
		if (raycast.collider->GetAttribute() == COLLISION_ATTR_OBJECT_MESH)
		{
			if (raycast.obj) ePos = raycast.obj->GetPosition();
			if (raycast.fbx) ePos = raycast.fbx->GetPosition();
			inter = ePos;
		}
		else
		{
			inter.x = raycast.inter.m128_f32[0];
			inter.y = raycast.inter.m128_f32[1];
			inter.z = raycast.inter.m128_f32[2];
		}

		// ビームエフェクト
		ParticleEmitter::Spark(32, 8, 4, inter, cannonPos, -0.1f,
			{ 0.0f,0.0f,1.0f,1.0f }, { 0.1f,0.1f,0.1f,1.0f }, 1.2f, 0.0f, 0.0f, 0.5f, 0.5f);
		ParticleEmitter::LaserBeam(16, 8, cannonPos, inter, 0.5f,
			{ 1.0f,0.2f,0.2f,0.01f }, { 1.0f,0.2f,0.2f,0.01f }, 0.0f, 0.01f, 2.0f, 2.0f);
	}

	attackCount--;

	if(attackCount <= -300)
	{
		attackCount = attackCountMax;
	}
}

void Enemy::Hit(float attackPower)
{
	HP -= attackPower;
	// 被弾したら無敵時間をリセット
	noDamageTime = noDamageTimeMax;
}

void Enemy::Defeated()
{
	if (HP <= 0)
	{
		aliveFlag = false;
		sphereColl->SetAttribute(COLLISION_ATTR_ENEMIES + 1);
		SetPosition(pos);
		ParticleEmitter::EmitRandomAllRange(
			8, 20, { pos.x,pos.y + radius,pos.z }, { 0, 0, 0 },
			{ 0.1f,0.1f,0.1f,1.0f }, { 0.0f,0.0f,3.0f,1.0f },
			0.5f, 0.01f, 1.0f, 0.0f);
	}
}

XMFLOAT3 Enemy::GetPosition()
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

void Enemy::OnCollision(const CollisionInfo& info)
{

	if (info.collider->GetAttribute() == COLLISION_ATTR_OBJECT_MESH && type != ROUTE_SEARCH)
	{
		XMFLOAT3 pos = {
			info.inter.m128_f32[0],
			info.inter.m128_f32[1],
			info.inter.m128_f32[2]
		};

		targetPos = pos;
	}
	else if (info.collider->GetAttribute() == COLLISION_ATTR_OBJECT_NONE)
	{
		SetTargetPos(target);
	}
	if (info.collider->GetAttribute() == (COLLISION_ATTR_WEAPONS + COLLISION_ATTR_ALLIES) ||
		info.collider->GetAttribute() == (COLLISION_ATTR_BULLET + COLLISION_ATTR_ALLIES))
	{
		if (noDamageTime == 0)
		{
			Hit(info.collider->attackPower);

			ParticleEmitter::EmitY_AxisDir(10, 30,
				{ info.inter.m128_f32[0], info.inter.m128_f32[1], info.inter.m128_f32[2] },
				0.0f,
				{ 0.5f, 0.5f ,0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f },
				0.05f, 0.001f, 1.0f, 0.0f);
		}
	}
}

void Enemy::SetPosition(XMFLOAT3 pos)
{
	this->pos = pos;
	// コライダーの追加
	sphere.center = XMLoadFloat3(&pos);
	sphere.center.m128_f32[1] += radius;
	sphere.radius = radius;

	sphereColl->SetSphere(sphere);

	if (ObjectOBJ::model)
	{
		ObjectOBJ::SetCollider(sphereColl);
		ObjectOBJ::SetPosition(pos);
		ObjectOBJ::collider = sphereColl;
	}
	if (ObjectFBX::model)
	{
		ObjectFBX::SetCollider(sphereColl);
		ObjectFBX::SetPosition(pos);
		ObjectFBX::collider = sphereColl;
	}
}

void Enemy::SetRotation(XMFLOAT3 rot)
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

void Enemy::SetScale(XMFLOAT3 scale)
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

void Enemy::SetStatus(float HP, float speed, float power, float radius)
{
	this->HP = HP;
	HPMax = HP;
	HPRate = HP / HPMax;
	this->moveDefault = { 0, 0, 0.1f * speed, 0 };
	this->attackPower = power;
	this->radius = radius;
	SetScale({ radius,radius,radius });
	sphereColl->SetAttribute(COLLISION_ATTR_ENEMIES);
}

void Enemy::SetType(EnemyType enemyType)
{
	this->type = enemyType;

	switch (type)
	{
	case FLYING:
		SetStatus(100.0f, 0.5f, 1.0f, 5.0f);
		break;

	case STRAIGHT:
	default:
		SetStatus(100.0f, 1.5f, 0.1f, 3.0f);
		break;
	}
}

void Enemy::SetWeapon(Weapon* weapon)
{
	this->weapon = weapon;

	if (ObjectOBJ::model)
	{
		this->weapon->SetParent((ObjectOBJ*)this);
	}
	if (ObjectFBX::model)
	{
		this->weapon->SetParent((ObjectFBX*)this);
	}

	this->weapon->SetPosition(pos);
	this->sphereColl->SetPower(GetPower());
}
