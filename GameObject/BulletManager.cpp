#include "BulletManager.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "CollisionManager.h"

static const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const DirectX::XMVECTOR& rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x + rhs.m128_f32[0];
	result.y = lhs.y + rhs.m128_f32[1];
	result.z = lhs.z + rhs.m128_f32[2];
	return result;
}

BulletManager* BulletManager::GetInstance()
{
	static BulletManager instance;
	return &instance;
}

void BulletManager::Update()
{
	// �������s�����e��S�폜
	bullets.remove_if([](Bullet& x) { return x.frame >= x.end_frame; });

	// �S�e�X�V
	for (std::forward_list<Bullet>::iterator it = bullets.begin();
		it != bullets.end(); it++)
	{
		it->frame++;
		it->position = it->position + it->velocity;
		ParticleEmitter::EmitAllRange(1, 6, it->position, { 0,0,0 });
		it->Update();
		CollisionManager::GetInstance()->CheckAllCollision(it->GetSphereCollider());

		if (it->frame >= it->end_frame) 
		{
			// �������s������R���C�_�[���폜
			CollisionManager::GetInstance()->RemoveCollider(it->GetSphereCollider());
		}
	}

}

void BulletManager::Fire(XMFLOAT3 position, XMFLOAT3 rotation, unsigned short collisionAttribute, float velocityMagnification, float power)
{
	// ���X�g�ɗv�f��ǉ�
	bullets.emplace_front();
	// �ǉ������v�f�̎Q��
	Bullet& bullet = bullets.front();
	bullet.position = position;
	bullet.rotation = rotation;
	bullet.velocity *= velocityMagnification;
	// �p�x���瑬�x�̃x�N�g�����v�Z
	XMMATRIX matRotX = XMMatrixRotationX(XMConvertToRadians(rotation.x));
	XMMATRIX matRotY = XMMatrixRotationY(XMConvertToRadians(rotation.y));
	bullet.velocity = XMVector3TransformNormal(bullet.velocity, matRotX);
	bullet.velocity = XMVector3TransformNormal(bullet.velocity, matRotY);
	bullet.collAttribute += collisionAttribute;
	bullet.Initialize();
	bullet.GetSphereCollider()->SetPower(power);
}

void BulletManager::Delete()
{
	bullets.clear();
}
