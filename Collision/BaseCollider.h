#pragma once

#include "CollisionTypes.h"
#include "CollisionInfo.h"
#include "ObjectOBJ.h"
#include "ObjectFBX.h"

/// <summary>
/// コライダー基底クラス
/// </summary>
class BaseCollider
{
public:
	friend class CollisionManager;

	BaseCollider() = default;
	virtual ~BaseCollider() = default;

	inline void SetObjectOBJ(ObjectOBJ* obj) {
		this->obj = obj;
	}

	inline ObjectOBJ* GetObjectOBJ() {
		return obj;
	}

	inline void SetObjectFBX(ObjectFBX* fbx) {
		this->fbx = fbx;
	}

	inline ObjectFBX* GetObjectFBX() {
		return fbx;
	}

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	inline CollisionShapeType GetShapeType() { return shapeType; }

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	inline void OnCollision(const CollisionInfo& info)
	{
		this->obj->OnCollision(info);
		this->fbx->OnCollision(info);
	}

	inline void SetAttribute(unsigned short attribute)
	{
		this->attribute = attribute;
	}
	inline void AddAttribute(unsigned short attribute)
	{
		this->attribute |= attribute;
	}
	inline void RemoveAttribute(unsigned short attribute)
	{
		this->attribute &= attribute;
	}

	inline unsigned short GetAttribute()
	{
		return attribute;
	}

protected:
	ObjectOBJ* obj = nullptr;
	ObjectFBX* fbx = nullptr;
	// 形状タイプ
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
	// 当たり判定属性
	unsigned short attribute = 0b1111'1111'1111'1111;
};

