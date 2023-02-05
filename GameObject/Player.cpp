#include "Player.h"
#include "Input.h"
#include "SphereCollider.h"
#include "Collision.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "ParticleEmitter.h"
#include "BulletManager.h"

Player* Player::Create(ModelFBX* fbx, int animationNumber)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Player* instance = new Player();
	if (instance == nullptr)
	{
		return nullptr;
	}

	// ���f���̃Z�b�g
	if (fbx)
	{
		instance->SetModelFBX(fbx);
		instance->SetAnimationNumber(animationNumber);
		instance->AnimationReset();
	}

	// ������
	if (!instance->Initialize())
	{
		delete instance;
		assert(0);
	}

	return instance;
}

Player* Player::Create(ModelOBJ* obj)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Player* instance = new Player();
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

bool Player::Initialize()
{
	ObjectFBX::Initialize();
	ObjectOBJ::Initialize();

	// �R���C�_�[�̒ǉ�
	sphere.center = XMLoadFloat3(&pos);
	sphere.center.m128_f32[1] += radius;
	sphere.radius = radius;
	sphereColl = new SphereCollider(sphere);

	if (ObjectOBJ::model) ObjectOBJ::SetCollider(sphereColl);
	if (ObjectFBX::model) ObjectFBX::SetCollider(sphereColl);
	sphereColl->SetAttribute(COLLISION_ATTR_ALLIES);

	return true;
}

void Player::Update()
{
	ControlCamera();

	if (!aliveFlag) return;

	if (ObjectOBJ::model) pos = ObjectOBJ::position;
	if (ObjectFBX::model) pos = ObjectFBX::position;

	Move();
	Jump();
	Attack();

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

	if(attackFlag)
	{
		ParticleEmitter::EmitRandomAllRange(5 ,attackCount, weapon->GetPosition(),
			{ 0,0,0 },
			{ 0.6f,0.6f,1.0f,1.0f }, { 0.0f,0.0f,1.0f,1.0f }, 0.1f, 0.001f, 2.0f);
	}

	ObjectOBJ::Update();
	ObjectFBX::Update();
}

void Player::Move()
{
	Input* input = Input::GetInstance();

	//�ړ��x�N�g����y������̊p�x�ŉ�]
	move = move_default;
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rot.y));
	move = XMVector3TransformNormal(move, matRot);

	//�����Ă�������Ɉړ�
	if (input->PushKey(DIK_S) || input->PushKey(DIK_W) ||
		input->PushKey(DIK_D) || input->PushKey(DIK_A) )
	{
		if (input->PushKey(DIK_W))
		{
			rot.y = 0 + cameraRot.y;
		}
		if (input->PushKey(DIK_D))
		{
			rot.y = 90 + cameraRot.y;
		}
		if (input->PushKey(DIK_S))
		{
			rot.y = 180 + cameraRot.y;
		}
		if (input->PushKey(DIK_A))
		{
			rot.y = 270 + cameraRot.y;
		}
		if (input->PushKey(DIK_W) && input->PushKey(DIK_D))
		{
			rot.y = 45 + cameraRot.y;
		}
		if (input->PushKey(DIK_D) && input->PushKey(DIK_S))
		{
			rot.y = 135 + cameraRot.y;
		}
		if (input->PushKey(DIK_S) && input->PushKey(DIK_A))
		{
			rot.y = 225 + cameraRot.y;
		}
		if (input->PushKey(DIK_A) && input->PushKey(DIK_W))
		{
			rot.y = 315 + cameraRot.y;
		}

		pos.x += move.m128_f32[0];
		pos.y += move.m128_f32[1];
		pos.z += move.m128_f32[2];
	}

	CollisionManager::GetInstance()->CheckAllCollision(sphereColl, COLLISION_ATTR_OBJECT_MESH);
}

void Player::Jump()
{
	// ��������
	if (!onGround)
	{
		// �����������x
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		// ����
		fallVel.m128_f32[1] = max(fallVel.m128_f32[1] + fallAcc, fallVYMin);
		// �ړ�
		pos.x += fallVel.m128_f32[0];
		pos.y += fallVel.m128_f32[1];
		pos.z += fallVel.m128_f32[2];
	}
	// �W�����v����
	else if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		onGround = false;
		const float jumpVYFist = 0.2f;
		fallVel = { 0, jumpVYFist, 0, 0 };
	}

	// ���̏�[���狅�̉��[�܂ł̃��C�L���X�g
	Ray ray;
	ray.start = XMLoadFloat3(&pos);
	ray.start.m128_f32[1] += sphereColl->GetRadius() * 2.0f;
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;

	// �ڒn���
	if (onGround)
	{
		// �X���[�Y�ɍ������ׂ̋z������
		const float adsDistance = 0.2f;
		// �ڒn���ێ�
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereColl->GetRadius() * 2.0f + adsDistance) ||
			CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_OBJECT_MESH, &raycastHit, sphereColl->GetRadius() * 2.0f + adsDistance))
		{
			onGround = true;
			sphereColl->center.m128_f32[1] = raycastHit.distance - sphereColl->GetRadius();
			pos.y -= sphereColl->center.m128_f32[1] - sphereColl->GetRadius();
		}
		// �n�ʂ��Ȃ��̂ŗ���
		else
		{
			onGround = false;
			fallVel = {};
		}
	}
	// �������
	else if (fallVel.m128_f32[1] <= 0.0f)
	{
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereColl->GetRadius() * 2.0f) ||
			CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_OBJECT_MESH, &raycastHit, sphereColl->GetRadius() * 2.0f))
		{
			// ���n
			onGround = true;
			sphereColl->center.m128_f32[1] = raycastHit.distance - sphereColl->GetRadius();
			pos.y -= sphereColl->center.m128_f32[1] - sphereColl->GetRadius();
		}
	}
}

void Player::ControlCamera()
{
	Input* input = Input::GetInstance();

	cameraPos = ObjectOBJ::GetCamera()->GetEye();

	cameraRot.y = XMConvertToDegrees(
		atan2f(pos.x - ObjectOBJ::GetCamera()->GetEye().x,
			pos.z - ObjectOBJ::GetCamera()->GetEye().z));

	if (cameraRotX < -70)
	{
		cameraRotX = -70;
	}
	if (cameraRotX > 70)
	{
		cameraRotX = 70;
	}

	cameraPos.x = pos.x + distance * cos(XMConvertToRadians(cameraRotY - 90)) * cos(XMConvertToRadians(cameraRotX));
	cameraPos.z = pos.z + distance * sin(XMConvertToRadians(cameraRotY - 90)) * cos(XMConvertToRadians(cameraRotX));

	cameraPos.y = pos.y + 2 + distance * sin(XMConvertToRadians(cameraRotX));

	//�L�[�{�[�h����
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) ||
		input->PushKey(DIK_LEFT) || input->PushKey(DIK_RIGHT))
	{
		if (input->PushKey(DIK_UP))
		{
			cameraRotX--;
		}
		else if (input->PushKey(DIK_DOWN))
		{
			cameraRotX++;
		}
		if (input->PushKey(DIK_LEFT))
		{
			cameraRotY++;
		}
		else if (input->PushKey(DIK_RIGHT))
		{
			cameraRotY--;
		}
	}

	if (cameraMoveOn)
	{
		//�}�E�X����
		cameraRotX += input->GetMouseMovement().y * 0.1f;		//�㉺�ړ�
		cameraRotY -= input->GetMouseMovement().x * 0.1f;		//���E�ړ�
	}

	ObjectOBJ::GetCamera()->SetEye(cameraPos);
}

void Player::Attack()
{
	Input* input = Input::GetInstance();

	cameraPos = ObjectOBJ::GetCamera()->GetEye();

	// �U��
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

			BulletManager::GetInstance()->Fire(pos, rot, COLLISION_ATTR_ALLIES, 1.0f, power);
		}
	}

	if (attackFlag == true) {

		attackCount -= 1.0f;

		if (attackCount < 14.0f && attackCount > -4.0f)
		{
			pos.x += move.m128_f32[0] * attackMove;
			pos.z += move.m128_f32[2] * attackMove;
		}

		// �J�E���g0�ōU�����t���O��false�ɂ���
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
		Rejection(info);
	}
}

void Player::Rejection(const CollisionInfo& info)
{
	pos.x += info.reject.m128_f32[0] / 2;
	pos.z += info.reject.m128_f32[2] / 2;

	SetPosition(pos);
}

void Player::SetPosition(XMFLOAT3 pos)
{
	// �R���C�_�[�̒ǉ�
	sphere.center = XMLoadFloat3(&pos);
	sphere.center.m128_f32[1] += radius;
	sphere.radius = radius;

	sphereColl->SetSphere(sphere);

	if (ObjectOBJ::model)
	{
		ObjectOBJ::SetPosition(pos);
		ObjectOBJ::collider = sphereColl;
	}
	if (ObjectFBX::model)
	{
		ObjectFBX::SetPosition(pos);
		ObjectFBX::collider = sphereColl;
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
