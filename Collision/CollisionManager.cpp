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

void CollisionManager::CheckAllCollision(BaseCollider* col)
{
	std::forward_list<BaseCollider*>::iterator it;

	//�S�Ă̑g�ݍ��킹�ɂ��đ�������`�F�b�N
	it = colliders.begin();
	for (; it != colliders.end(); ++it)
	{
		BaseCollider* colB = *it;

		CheckCollision(col, colB);
	}
}

void CollisionManager::CheckAllCollision(BaseCollider* col, unsigned short attr)
{
	std::forward_list<BaseCollider*>::iterator it;

	//�S�Ă̑g�ݍ��킹�ɂ��đ�������`�F�b�N
	it = colliders.begin();
	for (; it != colliders.end(); ++it)
	{
		BaseCollider* colB = *it;

		if (col == colB) continue;
		if (colB->GetAttribute() != attr) continue;

		CheckCollision(col, colB);
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
		XMVECTOR inter;
		XMVECTOR reject;

		if (Collision::CheckShpere2Sphere(*sphereA, *sphereB, &inter, &reject))
		{
			CheckSetObject(colA, colB, inter, reject);

			return true;
		}
	}
	// �Ƃ��ɋ�`(���Ő���)
	else if (colA->GetShapeType() == COLLISIONSHAPE_SQUARE &&
		colB->GetShapeType() == COLLISIONSHAPE_SQUARE)
	{
		Sphere* sphereA = dynamic_cast<Sphere*>(colA);
		Sphere* sphereB = dynamic_cast<Sphere*>(colB);
		XMVECTOR inter;
		XMVECTOR reject;

		if (Collision::CheckSquare2Square(*sphereA, *sphereB, &inter, &reject))
		{
			CheckSetObject(colA, colB, inter, reject);

			return true;
		}
	}
	//���b�V���Ƌ�
	else if (colA->GetShapeType() == COLLISIONSHAPE_MESH &&
		colB->GetShapeType() == COLLISIONSHAPE_SPHERE)
	{
		MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
		Sphere* sphere = dynamic_cast<Sphere*>(colB);
		XMVECTOR inter;

		if (meshCollider->CheckCollisionSphere(*sphere, &inter))
		{
			CheckSetObject(colA, colB, inter);

			return true;
		}
	}
	//���ƃ��b�V��
	else if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
		colB->GetShapeType() == COLLISIONSHAPE_MESH)
	{
		Sphere* sphere = dynamic_cast<Sphere*>(colA);
		MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
		XMVECTOR inter;

		if (meshCollider->CheckCollisionSphere(*sphere, &inter))
		{
			CheckSetObject(colA, colB, inter);

			return true;
		}
	}

	return false;
}

void CollisionManager::CheckSetObject(BaseCollider* colA, BaseCollider* colB, XMVECTOR inter, XMVECTOR reject)
{
	// A��B�Ƃ���OBJ
	if (colA->obj && colB->obj)
	{
		colA->OnCollision(CollisionInfo(colB->GetObjectOBJ(), colB, inter, reject));
		colB->OnCollision(CollisionInfo(colA->GetObjectOBJ(), colA, inter, reject));

		return;
	}
	// A��OBJ�AB��FBX
	if (colA->obj && colB->fbx)
	{
		colA->OnCollision(CollisionInfo(colB->GetObjectFBX(), colB, inter, reject));
		colB->OnCollision(CollisionInfo(colA->GetObjectOBJ(), colA, inter, reject));

		return;
	}
	// A��FBX�AB��OBJ
	if (colA->fbx && colB->obj)
	{
		colA->OnCollision(CollisionInfo(colB->GetObjectOBJ(), colB, inter, reject));
		colB->OnCollision(CollisionInfo(colA->GetObjectFBX(), colA, inter, reject));

		return;
	}
	// A��B�Ƃ���FBX
	if (colA->fbx && colB->fbx)
	{
		colA->OnCollision(CollisionInfo(colB->GetObjectFBX(), colB, inter, reject));
		colB->OnCollision(CollisionInfo(colA->GetObjectFBX(), colA, inter, reject));

		return;
	}
	if (!colA->fbx && !colA->obj && colB->obj)
	{
		colA->OnCollision(CollisionInfo(colB->GetObjectOBJ(), colB, inter, reject));

		return;
	}
	if (!colA->fbx && !colA->obj && colB->fbx)
	{
		colA->OnCollision(CollisionInfo(colB->GetObjectFBX(), colB, inter, reject));

		return;
	}
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
		if (colA->attribute != attribute)
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
			hitInfo->obj = hitInfo->collider->GetObjectOBJ();
			hitInfo->fbx = hitInfo->collider->GetObjectFBX();
		}
	}

	return result;
}
