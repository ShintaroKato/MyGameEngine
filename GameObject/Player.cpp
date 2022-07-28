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

	// ������
	if (!instance->Initialize())
	{
		delete instance;
		assert(0);
	}

	// ���f���̃Z�b�g
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
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Player* instance = new Player();
	if (instance == nullptr)
	{
		return nullptr;
	}

	// ������
	if (!instance->Initialize())
	{
		delete instance;
		assert(0);
	}

	// ���f���̃Z�b�g
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

	// �R���C�_�[�̒ǉ�
	float radius = 0.6f;
	collider = new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius);

	// ���a�������������畂�������W�����̒��S�ɂ���
	if(ObjectOBJ::model) ObjectOBJ::SetCollider(collider);
	if(ObjectFBX::model) ObjectFBX::SetCollider(collider);
	collider->SetAttribute(COLLISION_ATTR_ALLIES);

	return true;
}

void Player::Update()
{
	if (ObjectOBJ::model) position = ObjectOBJ::position;
	if (ObjectFBX::model) position = ObjectFBX::position;

	Move();
	Attack();

	if (ObjectOBJ::model)
	{
		ObjectOBJ::position = position;
		ObjectOBJ::rotation = rotation;
	}
	if (ObjectFBX::model)
	{
		ObjectFBX::position = position;
		ObjectFBX::rotation = rotation;
	}
	ObjectOBJ::Update();
	ObjectFBX::Update();
}

void Player::Move()
{
	Input* input = Input::GetInstance();

	cameraPos = ObjectOBJ::GetCamera()->GetEye();

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

	//�ړ��x�N�g����y������̊p�x�ŉ�]
	move = { 0,0,0.1f,0 };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotation.y));
	move = XMVector3TransformNormal(move, matRot);


	cameraRot.y = XMConvertToDegrees(
		atan2f(position.x - ObjectOBJ::GetCamera()->GetEye().x,
			position.z - ObjectOBJ::GetCamera()->GetEye().z));

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

		position.x += move.m128_f32[0];
		position.y += move.m128_f32[1];
		position.z += move.m128_f32[2];
	}

	cameraPos.x = position.x + 10 * cos(XMConvertToRadians(cameraRotY));
	cameraPos.z = position.z + 10 * sin(XMConvertToRadians(cameraRotY));

	ObjectOBJ::GetCamera()->SetEye(cameraPos);

	//��������
	if (!onGround)
	{
		//�����������x
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		//����
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
		//�ړ�
		position.x += fallV.m128_f32[0];
		position.y += fallV.m128_f32[1];
		position.z += fallV.m128_f32[2];
	}
	//�W�����v����
	else if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		onGround = false;
		const float jumpVYFist = 0.2f; //�W�����v�����������
		fallV = { 0,jumpVYFist,0,0 };
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

void Player::OnCollision(const CollisionInfo& info)
{

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
