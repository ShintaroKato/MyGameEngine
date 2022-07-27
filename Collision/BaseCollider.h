#pragma once

#include "CollisionTypes.h"
#include "CollisionInfo.h"
#include "ObjectOBJ.h"
#include "ObjectFBX.h"

/// <summary>
/// �R���C�_�[���N���X
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
	/// �X�V
	/// </summary>
	virtual void Update() = 0;

	inline CollisionShapeType GetShapeType() { return shapeType; }

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
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
	// �`��^�C�v
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
	// �����蔻�葮��
	unsigned short attribute = 0b1111'1111'1111'1111;
};

