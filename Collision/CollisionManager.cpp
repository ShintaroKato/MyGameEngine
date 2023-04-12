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

			if (colA == colB) continue;

			if (CheckCollision(colA, colB)) return true;
		}
	}

	return false; 
}

bool CollisionManager::CheckAllCollision(BaseCollider* col)
{
	std::forward_list<BaseCollider*>::iterator it;

	//全ての組み合わせについて総当たりチェック
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

	//全ての組み合わせについて総当たりチェック
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

	//全ての組み合わせについて総当たりチェック
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
	//ともに球
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
	// ともに矩形(球で生成)
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
	// AがOBJ,FBXではなく、BがOBJ
	if (colA->none && colB->obj)
	{
		colA->OnCollision(CollisionInfo(colB->GetObjectOBJ(), colB, inter, reject));
		colB->OnCollision(CollisionInfo(colA, inter, reject));

		return;
	}
	// AがOBJ,FBXではなく、BがFBX
	if (colA->none && colB->fbx)
	{
		colA->OnCollision(CollisionInfo(colB->GetObjectFBX(), colB, inter, reject));
		colB->OnCollision(CollisionInfo(colA, inter, reject));

		return;
	}
	// BがOBJ,FBXではなく、AがOBJ
	if (colB->none && colA->obj)
	{
		colA->OnCollision(CollisionInfo(colB, inter, reject));
		colB->OnCollision(CollisionInfo(colA->GetObjectOBJ(), colA, inter, reject));

		return;
	}
	// BがOBJ,FBXではなく、AがFBX
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
		if (colA->attribute != attribute && attribute != 0xffff)
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

bool CollisionManager::Raycast(const Ray& ray, unsigned short ex_attribute, BaseCollider* ex_col, RaycastHit* hitInfo, float maxDistance)
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

		//属性 または コライダー が一致していたらスキップ
		if (colA->attribute == ex_attribute || colA == ex_col || colA->none != nullptr)
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

void CollisionManager::QuerySphere(const Sphere& sphere, QueryCallback* callback, unsigned short attribute)
{
	assert(callback);
	std::forward_list<BaseCollider*>::iterator it;

	// 全てのコライダーと総当たりチェック
	it = colliders.begin();

	for (; it != colliders.end(); ++it)
	{
		BaseCollider* col = *it;

		// 属性が合わなければスキップ
		if (!(col->attribute & attribute))
		{
			continue;
		}

		// 球
		if (col->GetShapeType() == COLLISIONSHAPE_SPHERE)
		{
			Sphere* sphereB = dynamic_cast<Sphere*>(col);

			XMVECTOR tempInter;
			XMVECTOR tempReject;

			if (!Collision::CheckSphere2Sphere(sphere, *sphereB, &tempInter, &tempReject)) continue;

			// 交差情報セット
			QueryHit info;
			if (col->GetObjectOBJ()) info.obj = col->GetObjectOBJ();
			if (col->GetObjectFBX()) info.fbx = col->GetObjectFBX();
			info.collider = col;
			info.inter = tempInter;
			info.reject = tempReject;

			// クエリコールバック呼び出し、戻り値がfalseの場合、継続せず終了
			if (!callback->OnQueryHit(info)) return;
		}
		// メッシュ
		else if (col->GetShapeType() == COLLISIONSHAPE_MESH)
		{
			MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(col);

			XMVECTOR tempInter;
			XMVECTOR tempReject;

			if (!meshCollider->CheckCollisionSphere(sphere, &tempInter, &tempReject)) continue;

			// 交差情報セット
			QueryHit info;
			if (col->GetObjectOBJ()) info.obj = col->GetObjectOBJ();
			if (col->GetObjectFBX()) info.fbx = col->GetObjectFBX();
			info.collider = col;
			info.inter = tempInter;
			info.reject = tempReject;

			// クエリコールバック呼び出し、戻り値がfalseの場合、継続せず終了
			if (!callback->OnQueryHit(info)) return;
		}
	}
}
