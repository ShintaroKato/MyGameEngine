#include "ObjectNone.h"
#include "BaseCollider.h"
#include "CollisionManager.h"

void ObjectNone::SetCollider(BaseCollider* collider)
{
	collider->SetObjectNone(this);
	this->collider = collider;
	// コリジョンマネージャに追加
	CollisionManager::GetInstance()->AddCollider(this->collider);
	// コライダーを更新しておく
	this->collider->Update();
}
