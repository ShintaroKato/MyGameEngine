#include "BulletManager.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "CollisionManager.h"

static const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const DirectX::XMVECTOR& rhs)
{
	XMFLOAT3 result{};
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
	// 寿命が尽きた弾を全削除
	bullets.remove_if([](Bullet& x) { return x.frame >= x.end_frame; });

	// 全弾更新
	for (std::forward_list<Bullet>::iterator it = bullets.begin();
		it != bullets.end(); it++)
	{
		it->frame++;
		it->position = it->position + it->velocity;
		ParticleEmitter::EmitRandomAllRange(1, 6, it->position, { 0,0,0 });
		it->Update();
		CollisionManager::GetInstance()->CheckAllCollision(it->GetSphereCollider(), it->ex_col);

		if (it->frame >= it->end_frame) 
		{
			// 寿命が尽きたらコライダーを削除
			CollisionManager::GetInstance()->RemoveCollider(it->GetSphereCollider());
		}
	}

}

void BulletManager::Fire(XMFLOAT3 position, XMFLOAT3 rotation, unsigned short collisionAttribute, float velocityMagnification, float power, bool through)
{
	// リストに要素を追加
	bullets.emplace_front();
	// 追加した要素の参照
	Bullet& bullet = bullets.front();
	bullet.position = position;
	bullet.rotation = rotation;
	// 角度から速度のベクトルを計算
	XMMATRIX matRotX = XMMatrixRotationX(XMConvertToRadians(rotation.x));
	XMMATRIX matRotY = XMMatrixRotationY(XMConvertToRadians(rotation.y));
	//XMMATRIX matRotZ = XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	bullet.velocity = XMVector3TransformNormal(bullet.velocity, matRotX);
	bullet.velocity = XMVector3TransformNormal(bullet.velocity, matRotY);
	//bullet.velocity = XMVector3TransformNormal(bullet.velocity, matRotZ);
	bullet.velocity *= velocityMagnification;
	bullet.collAttribute += collisionAttribute;
	bullet.Initialize();
	bullet.GetSphereCollider()->SetPower(power);
	bullet.through = through;
}

void BulletManager::Fire(XMFLOAT3 position, XMFLOAT3 rotation, unsigned short collisionAttribute, BaseCollider* ex_col, float velocityMagnification, float power, bool through)
{
	// リストに要素を追加
	bullets.emplace_front();
	// 追加した要素の参照
	Bullet& bullet = bullets.front();
	bullet.position = position;
	bullet.rotation = rotation;
	// 角度から速度のベクトルを計算
	XMMATRIX matRotX = XMMatrixRotationX(XMConvertToRadians(rotation.x));
	XMMATRIX matRotY = XMMatrixRotationY(XMConvertToRadians(rotation.y));
	//XMMATRIX matRotZ = XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	bullet.velocity = XMVector3TransformNormal(bullet.velocity, matRotX);
	bullet.velocity = XMVector3TransformNormal(bullet.velocity, matRotY);
	//bullet.velocity = XMVector3TransformNormal(bullet.velocity, matRotZ);
	bullet.velocity *= velocityMagnification;
	bullet.collAttribute += collisionAttribute;
	bullet.ex_col = ex_col;
	bullet.Initialize();
	bullet.GetSphereCollider()->SetPower(power);
	bullet.through = through;
}

void BulletManager::Delete()
{
	bullets.clear();
}
