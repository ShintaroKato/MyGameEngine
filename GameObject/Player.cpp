#include "Player.h"
#include "Input.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

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
	sphere.center = { pos.x, pos.y + radius, pos.z,0 };
	sphere.radius = radius;
	sphereColl = new SphereCollider(sphere);

	if (ObjectOBJ::model) ObjectOBJ::SetCollider(sphereColl);
	if (ObjectFBX::model) ObjectFBX::SetCollider(sphereColl);
	sphereColl->SetAttribute(COLLISION_ATTR_OBJECT);

	return true;
}

void Player::Update()
{
	ControlCamera();

	if (!alliveFlag) return;

	if (ObjectOBJ::model) pos = ObjectOBJ::position;
	if (ObjectFBX::model) pos = ObjectFBX::position;

	Move();
	Attack();

	if (ObjectOBJ::model)
	{
		ObjectOBJ::position = pos;
		ObjectOBJ::rotation = rotation;
	}
	if (ObjectFBX::model)
	{
		ObjectFBX::position = pos;
		ObjectFBX::rotation = rotation;
	}
	ObjectOBJ::Update();
	ObjectFBX::Update();
}

void Player::Move()
{
	Input* input = Input::GetInstance();

	//�ړ��x�N�g����y������̊p�x�ŉ�]
	move = { 0,0,0.1f,0 };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotation.y));
	move = XMVector3TransformNormal(move, matRot);

	//�����Ă�������Ɉړ�
	if (input->PushKey(DIK_S) || input->PushKey(DIK_W) ||
		input->PushKey(DIK_D) || input->PushKey(DIK_A) )
	{
		if (input->PushKey(DIK_W))
		{
			rotation.y = 0 + cameraRot.y;
		}
		if (input->PushKey(DIK_D))
		{
			rotation.y = 90 + cameraRot.y;
		}
		if (input->PushKey(DIK_S))
		{
			rotation.y = 180 + cameraRot.y;
		}
		if (input->PushKey(DIK_A))
		{
			rotation.y = 270 + cameraRot.y;
		}
		if (input->PushKey(DIK_W) && input->PushKey(DIK_D))
		{
			rotation.y = 45 + cameraRot.y;
		}
		if (input->PushKey(DIK_D) && input->PushKey(DIK_S))
		{
			rotation.y = 135 + cameraRot.y;
		}
		if (input->PushKey(DIK_S) && input->PushKey(DIK_A))
		{
			rotation.y = 225 + cameraRot.y;
		}
		if (input->PushKey(DIK_A) && input->PushKey(DIK_W))
		{
			rotation.y = 315 + cameraRot.y;
		}

		pos.x += move.m128_f32[0];
		pos.y += move.m128_f32[1];
		pos.z += move.m128_f32[2];
	}

	//��������
	if (!onGround)
	{
		//�����������x
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		//����
		fallVel.m128_f32[1] = max(fallVel.m128_f32[1] + fallAcc, fallVYMin);
		//�ړ�
		pos.x += fallVel.m128_f32[0];
		pos.y += fallVel.m128_f32[1];
		pos.z += fallVel.m128_f32[2];
	}
	//�W�����v����
	else if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		onGround = false;
		const float jumpVYFirst = 0.2f; //�W�����v�����������
		fallVel = { 0,jumpVYFirst,0,0 };
	}

}

void Player::ControlCamera()
{
	Input* input = Input::GetInstance();

	cameraPos = ObjectOBJ::GetCamera()->GetEye();

	cameraRot.y = XMConvertToDegrees(
		atan2f(pos.x - ObjectOBJ::GetCamera()->GetEye().x,
			pos.z - ObjectOBJ::GetCamera()->GetEye().z));

	cameraPos.x = pos.x + distance * cos(XMConvertToRadians(cameraRotY));
	cameraPos.z = pos.z + distance * sin(XMConvertToRadians(cameraRotY));

	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) ||
		input->PushKey(DIK_LEFT) || input->PushKey(DIK_RIGHT))
	{
		if (input->PushKey(DIK_UP))
		{
			cameraPos.y -= 0.1;
		}
		else if (input->PushKey(DIK_DOWN))
		{
			cameraPos.y += 0.1;
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

	ObjectOBJ::GetCamera()->SetEye(cameraPos);
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

void Player::OnCollision(const CollisionInfo& info)
{

}

void Player::SetPosition(XMFLOAT3 pos)
{
	// �R���C�_�[�̒ǉ�
	sphere.center = { pos.x, pos.y + radius, pos.z,0 };
	sphere.radius = radius;

	sphereColl->SetSphere(sphere);

	if (ObjectOBJ::model)
	{
		ObjectOBJ::SetPosition(pos);
		ObjectOBJ::SetCollider(sphereColl);
	}
	if (ObjectFBX::model)
	{
		ObjectFBX::SetPosition(pos);
		ObjectFBX::SetCollider(sphereColl);
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
