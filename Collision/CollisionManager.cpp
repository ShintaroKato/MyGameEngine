#include "CollisionManager.h"
#include "BaseCollider.h"
#include "MeshCollider.h"
#include "Collision.h"

using namespace DirectX;

CollisionManager* CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return &instance;
}

void CollisionManager::CheckAllCollision()
{
	std::forward_list<BaseCollider*>::iterator itA;
	std::forward_list<BaseCollider*>::iterator itB;

	//�S�Ă̑g�ݍ��킹�ɂ��đ�������`�F�b�N
	itA = colliders.begin();
	for (; itA != colliders.end(); ++itA)
	{
		itB = itA;
		++itB;

		for (; itB != colliders.end(); ++itB)
		{
			BaseCollider* colA = *itA;
			BaseCollider* colB = *itB;

			CheckCollision(colA, colB);
		}
	}
}

bool CollisionManager::CheckCollision(BaseCollider* colA, BaseCollider* colB)
{
	//�Ƃ��ɋ�
	if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
		colB->GetShapeType() == COLLISIONSHAPE_SPHERE)
	{
		Sphere* sphereA = dynamic_cast<Sphere*>(colA);
		Sphere* sphereB = dynamic_cast<Sphere*>(colB);
		DirectX::XMVECTOR inter;

		if (Collision::CheckShpere2Sphere(*sphereA, *sphereB, &inter))
		{
			colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
			colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));

			return true;
		}
	}
	//���b�V���Ƌ�
	else if (colA->GetShapeType() == COLLISIONSHAPE_MESH &&
		colB->GetShapeType() == COLLISIONSHAPE_SPHERE)
	{
		MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
		Sphere* sphere = dynamic_cast<Sphere*>(colB);
		DirectX::XMVECTOR inter;

		if (meshCollider->CheckCollisionSphere(*sphere, &inter))
		{
			colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
			colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));

			return true;
		}
	}
	//���ƃ��b�V��
	else if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
		colB->GetShapeType() == COLLISIONSHAPE_MESH)
	{
		Sphere* sphere = dynamic_cast<Sphere*>(colA);
		MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
		DirectX::XMVECTOR inter;

		if (meshCollider->CheckCollisionSphere(*sphere, &inter))
		{
			colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
			colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));

			return true;
		}
	}

	return false;
}

bool CollisionManager::Raycast(const Ray& ray, RaycastHit* hitInfo, float maxDistance)
{
	return Raycast(ray, 0xffff, hitInfo, maxDistance);
}

bool CollisionManager::Raycast(const Ray& ray, unsigned short attribute, RaycastHit* hitInfo, float maxDistance)
{
	bool result = false;
	//�����p�C�e���[�^
	std::forward_list<BaseCollider*>::iterator it;
	//���܂łōł��߂��R���C�_�[���L�^���邽�߂̃C�e���[�^
	std::forward_list<BaseCollider*>::iterator it_hit;
	//���܂łōł��߂��R���C�_�[�̋������L�^����ϐ�
	float distance = maxDistance;
	//���܂łōł��߂��R���C�_�[�Ƃ̌�_���L�^����ϐ�
	XMVECTOR inter;

	//�S�ẴR���C�_�[�Ƒ�������`�F�b�N
	it = colliders.begin();
	for (; it != colliders.end(); ++it)
	{
		BaseCollider* colA = *it;

		//����������Ȃ���΃X�L�b�v
		if (!(colA->attribute & attribute))
		{
			continue;
		}

		//���̏ꍇ
		if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE)
		{
			Sphere* sphere = dynamic_cast<Sphere*>(colA);
			float tempDistance;
			XMVECTOR tempInter;
			//������Ȃ���Ώ��O
			if (!Collision::ChackRay2Sphere(ray, *sphere, &tempDistance, &tempInter)) continue;
			//�������ŏ��łȂ���Ώ��O
			if (tempDistance >= distance) continue;
			//���܂łōł��߂��̂ŋL�^���Ƃ�
			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}
		//���b�V���̏ꍇ
		else if (colA->GetShapeType() == COLLISIONSHAPE_MESH)
		{
			MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
			float tempDistance;
			XMVECTOR tempInter;
			//������Ȃ���Ώ��O
			if (!meshCollider->CheckCollisionRay(ray, &tempDistance, &tempInter)) continue;
			//�������ŏ��łȂ���Ώ��O
			if (tempDistance >= distance) continue;
			//���܂łōł��߂��̂ŋL�^���Ƃ�
			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}
		//�ŏI�I�ɉ����ɓ������Ă����猋�ʂ���������
		if (result && hitInfo)
		{
			hitInfo->distance = distance;
			hitInfo->inter = inter;
			hitInfo->collider = *it_hit;
			hitInfo->object = hitInfo->collider->GetObject3d();
		}
	}

	return result;
}
