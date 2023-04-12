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

bool CollisionManager::CheckAllCollision()
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

			if (colA == colB) continue;

			if (CheckCollision(colA, colB)) return true;
		}
	}

	return false; 
}

bool CollisionManager::CheckAllCollision(BaseCollider* col)
{
	std::forward_list<BaseCollider*>::iterator it;

	//�S�Ă̑g�ݍ��킹�ɂ��đ�������`�F�b�N
	it = colliders.begin();
	for (; it != colliders.end(); ++it)
	{
		BaseCollider* colB = *it;

		if (col == colB) continue;
		if (col->none && colB->none) continue;

		if (CheckCollision(col, colB)) return true;
	}

	return false;
}

bool CollisionManager::CheckAllCollision(BaseCollider* col, unsigned short attr)
{
	bool result = false;

	std::forward_list<BaseCollider*>::iterator it;

	//�S�Ă̑g�ݍ��킹�ɂ��đ�������`�F�b�N
	it = colliders.begin();
	for (; it != colliders.end(); ++it)
	{
		BaseCollider* colB = *it;

		if (col == colB) continue;
		if (colB->GetAttribute() != attr) continue;
		if (col->none && colB->none) continue;

		if(CheckCollision(col, colB)) result = true;
	}

	return result;
}

bool CollisionManager::CheckAllCollision(BaseCollider* col, BaseCollider* ex_col)
{
	bool result = false;

	std::forward_list<BaseCollider*>::iterator it;

	//�S�Ă̑g�ݍ��킹�ɂ��đ�������`�F�b�N
	it = colliders.begin();
	for (; it != colliders.end(); ++it)
	{
		BaseCollider* colB = *it;

		if (col == colB) continue;
		if (colB == ex_col) continue;
		if (col->none && colB->none) continue;

		if (CheckCollision(col, colB)) result = true;
	}

	return result;
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

		if (Collision::CheckSphere2Sphere(*sphereA, *sphereB, &inter, &reject))
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

		if (Collision::CheckSquare2Square(sphereA->square, sphereB->square, &inter, &reject))
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
	// A��OBJ,FBX�ł͂Ȃ��AB��OBJ
	if (colA->none && colB->obj)
	{
		colA->OnCollision(CollisionInfo(colB->GetObjectOBJ(), colB, inter, reject));
		colB->OnCollision(CollisionInfo(colA, inter, reject));

		return;
	}
	// A��OBJ,FBX�ł͂Ȃ��AB��FBX
	if (colA->none && colB->fbx)
	{
		colA->OnCollision(CollisionInfo(colB->GetObjectFBX(), colB, inter, reject));
		colB->OnCollision(CollisionInfo(colA, inter, reject));

		return;
	}
	// B��OBJ,FBX�ł͂Ȃ��AA��OBJ
	if (colB->none && colA->obj)
	{
		colA->OnCollision(CollisionInfo(colB, inter, reject));
		colB->OnCollision(CollisionInfo(colA->GetObjectOBJ(), colA, inter, reject));

		return;
	}
	// B��OBJ,FBX�ł͂Ȃ��AA��FBX
	if (colB->none && colA->fbx)
	{
		colA->OnCollision(CollisionInfo(colB, inter, reject));
		colB->OnCollision(CollisionInfo(colA->GetObjectFBX(), colA, inter, reject));

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
		if (colA->attribute != attribute && attribute != 0xffff)
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

bool CollisionManager::Raycast(const Ray& ray, unsigned short ex_attribute, BaseCollider* ex_col, RaycastHit* hitInfo, float maxDistance)
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

		//���� �܂��� �R���C�_�[ ����v���Ă�����X�L�b�v
		if (colA->attribute == ex_attribute || colA == ex_col || colA->none != nullptr)
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

void CollisionManager::QuerySphere(const Sphere& sphere, QueryCallback* callback, unsigned short attribute)
{
	assert(callback);
	std::forward_list<BaseCollider*>::iterator it;

	// �S�ẴR���C�_�[�Ƒ�������`�F�b�N
	it = colliders.begin();

	for (; it != colliders.end(); ++it)
	{
		BaseCollider* col = *it;

		// ����������Ȃ���΃X�L�b�v
		if (!(col->attribute & attribute))
		{
			continue;
		}

		// ��
		if (col->GetShapeType() == COLLISIONSHAPE_SPHERE)
		{
			Sphere* sphereB = dynamic_cast<Sphere*>(col);

			XMVECTOR tempInter;
			XMVECTOR tempReject;

			if (!Collision::CheckSphere2Sphere(sphere, *sphereB, &tempInter, &tempReject)) continue;

			// �������Z�b�g
			QueryHit info;
			if (col->GetObjectOBJ()) info.obj = col->GetObjectOBJ();
			if (col->GetObjectFBX()) info.fbx = col->GetObjectFBX();
			info.collider = col;
			info.inter = tempInter;
			info.reject = tempReject;

			// �N�G���R�[���o�b�N�Ăяo���A�߂�l��false�̏ꍇ�A�p�������I��
			if (!callback->OnQueryHit(info)) return;
		}
		// ���b�V��
		else if (col->GetShapeType() == COLLISIONSHAPE_MESH)
		{
			MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(col);

			XMVECTOR tempInter;
			XMVECTOR tempReject;

			if (!meshCollider->CheckCollisionSphere(sphere, &tempInter, &tempReject)) continue;

			// �������Z�b�g
			QueryHit info;
			if (col->GetObjectOBJ()) info.obj = col->GetObjectOBJ();
			if (col->GetObjectFBX()) info.fbx = col->GetObjectFBX();
			info.collider = col;
			info.inter = tempInter;
			info.reject = tempReject;

			// �N�G���R�[���o�b�N�Ăяo���A�߂�l��false�̏ꍇ�A�p�������I��
			if (!callback->OnQueryHit(info)) return;
		}
	}
}
