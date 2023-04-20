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

		Move();
		Step();
		Jump();
		Attack();

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

		ObjectOBJ::Update(); 
		ObjectFBX::Update();
	}
}

void Player::Move()
{
	if (stepTimer < stepEnd) return;

	Input* input = Input::GetInstance();

	//�ړ��x�N�g����y������̊p�x�ŉ�]
	move = move_default;
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rot.y));
	move = XMVector3TransformNormal(move, matRot);

	//�����Ă�������Ɉړ�
	if (input->PushKey(DIK_S) || input->PushKey(DIK_W) ||
		input->PushKey(DIK_D) || input->PushKey(DIK_A) )
	{
		float cameraRotY = -camera->GetRotation().y;

		if (input->PushKey(DIK_W))
		{
			rot.y = 0 + cameraRotY;
		}
		if (input->PushKey(DIK_D))
		{
			rot.y = 90 + cameraRotY;
		}
		if (input->PushKey(DIK_S))
		{
			rot.y = 180 + cameraRotY;
		}
		if (input->PushKey(DIK_A))
		{
			rot.y = 270 + cameraRotY;
		}
		if (input->PushKey(DIK_W) && input->PushKey(DIK_D))
		{
			rot.y = 45 + cameraRotY;
		}
		if (input->PushKey(DIK_D) && input->PushKey(DIK_S))
		{
			rot.y = 135 + cameraRotY;
		}
		if (input->PushKey(DIK_S) && input->PushKey(DIK_A))
		{
			rot.y = 225 + cameraRotY;
		}
		if (input->PushKey(DIK_A) && input->PushKey(DIK_W))
		{
			rot.y = 315 + cameraRotY;
		}

		pos.x += move.m128_f32[0];
		pos.y += move.m128_f32[1];
		pos.z += move.m128_f32[2];
	}
}

void Player::Jump()
{
	if(stepTimer > stepEnd)
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

void Player::Step()
{
	Input* input = Input::GetInstance();

	if (input->TriggerKey(DIK_LSHIFT) && !isStepped)
	{
		move_step = move_step_default;
		stepTimer = 0;
		isStepped = true;
	}
	if (isStepped)
	{
		if (stepTimer < stepTimerMax)
		{
			stepTimer++;

			if (stepTimer < stepEnd)
			{
				move = move_step;
				XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rot.y));
				move = XMVector3TransformNormal(move, matRot);
				move_step.m128_f32[2] -= 0.1f;

				pos.x += move.m128_f32[0];
				pos.y += move.m128_f32[1];
				pos.z += move.m128_f32[2];
			}
		}
		else isStepped = false;
	}
}

void Player::Rejection()
{

	// �N�G���R�[���o�b�N�N���X
	class PlayerQueryCallback : public QueryCallback
	{
	public:
		PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

		bool OnQueryHit(const QueryHit& info)
		{
			// ���[���h�̏����
			const XMVECTOR up = { 0.1,0,0 };
			// �r�˕���
			XMVECTOR rejectDir = XMVector3Normalize(info.reject);
			// ������Ɣr�˃x�N�g���̃R�T�C���l
			float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

			// �n�ʔ��肵�����l�p�x
			const float threshold = cosf(XMConvertToRadians(30.0f));
			// �p�x���ɂ���ēV��܂��͒n�ʂƔ��肳���ꍇ��������
			if (-threshold < cos && cos < threshold)
			{
				// ����r��
				sphere->center += info.reject;
				move += info.reject;
			}

			return true;
		}

		// �N�G���Ɏg�p���鋅
		Sphere* sphere = nullptr;
		// �r�˂ɂ��ړ���(�ݐϒl)
		DirectX::XMVECTOR move = {};
	};

	// �N�G���R�[���o�b�N�̊֐��I�u�W�F�N�g
	PlayerQueryCallback callback(sphereColl);

	// ���ƒn�`�̌�����S����
	CollisionManager::GetInstance()->QuerySphere(*sphereColl, &callback, COLLISION_ATTR_OBJECT_MESH);
	// �����̂��r�˕�������
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

	// �R���C�_�[�X�V
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
	// �R���C�_�[�̒ǉ�
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
