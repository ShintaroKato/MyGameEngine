#pragma once
#include "ObjectOBJ.h"
#include "ObjectFBX.h"
#include "CollisionAttribute.h"

class TouchableObject : public ObjectOBJ, public ObjectFBX
{
public:

	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static TouchableObject* Create(ModelOBJ* obj = nullptr, ModelFBX* fbx = nullptr);

	/// <summary>
	/// ������
	/// </summary>
	/// <returns></returns>
	bool Initialize(ModelOBJ* obj, ModelFBX* fbx);
};