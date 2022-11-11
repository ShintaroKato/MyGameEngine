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

	//全ての組み合わせについて総当たりチェック
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

	//全ての組み合わせについて総当たりチェック
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

	//全ての組み合わせについて総当たりチェック
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
	//ともに球
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
	// ともに矩形(球で生成)
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
	//メッシュと球
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
	//球とメッシュ
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
	// AとBともにOBJ
	if (colA->obj && colB->obj)
	{
		colA->OnCollision(CollisionInfo(colB->GetObjectOBJ(), colB, inter, reject));
		colB->OnCollision(CollisionInfo(colA->GetObjectOBJ(), colA, inter, reject));

		return;
	}
	// AがOBJ、BがFBX
	if (colA->obj && colB->fbx)
	{
		colA->OnCollision(CollisionInfo(colB->GetObjectFBX(), colB, inter, reject));
		colB->OnCollision(CollisionInfo(colA->GetObjectOBJ(), colA, inter, reject));

		return;
	}
	// AがFBX、BがOBJ
	if (colA->fbx && colB->obj)
	{
		colA->OnCollision(CollisionInfo(colB->GetObjectOBJ(), colB, inter, reject));
		colB->OnCollision(CollisionInfo(colA->GetObjectFBX(), colA, inter, reject));

		return;
	}
	// AとBともにFBX
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
	//走査用イテレータ
	std::forward_list<BaseCollider*>::iterator it;
	//今までで最も近いコライダーを記録するためのイテレータ
	std::forward_list<BaseCollider*>::iterator it_hit;
	//今までで最も近いコライダーの距離を記録する変数
	float distance = maxDistance;
	//今までで最も近いコライダーとの交点を記録する変数
	XMVECTOR inter;

	//全てのコライダーと総当たりチェック
	it = colliders.begin();
	for (; it != colliders.end(); ++it)
	{
		BaseCollider* colA = *it;

		//属性が合わなければスキップ
		if (colA->attribute != attribute)
		{
			continue;
		}

		//球の場合
		if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE)
		{
			Sphere* sphere = dynamic_cast<Sphere*>(colA);
			float tempDistance;
			XMVECTOR tempInter;
			//当たらなければ除外
			if (!Collision::ChackRay2Sphere(ray, *sphere, &tempDistance, &tempInter)) continue;
			//距離が最小でなければ除外
			if (tempDistance >= distance) continue;
			//今までで最も近いので記録をとる
			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}
		//メッシュの場合
		else if (colA->GetShapeType() == COLLISIONSHAPE_MESH)
		{
			MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
			float tempDistance;
			XMVECTOR tempInter;
			//当たらなければ除外
			if (!meshCollider->CheckCollisionRay(ray, &tempDistance, &tempInter)) continue;
			//距離が最小でなければ除外
			if (tempDistance >= distance) continue;
			//今までで最も近いので記録をとる
			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}
		//最終的に何かに当たっていたら結果を書き込む
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
