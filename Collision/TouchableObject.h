#pragma once
#include "ObjectOBJ.h"
#include "ObjectFBX.h"
#include "CollisionAttribute.h"

class TouchableObject : public ObjectOBJ, public ObjectFBX
{
public:

	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <returns></returns>
	static TouchableObject* Create(ModelOBJ* obj = nullptr, ModelFBX* fbx = nullptr);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	bool Initialize(ModelOBJ* obj, ModelFBX* fbx);
};