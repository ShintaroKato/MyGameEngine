#pragma once
#include "Object3d.h"
#include "CollisionAttribute.h"

class TouchableObject : public Object3d
{
public:

	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <returns></returns>
	static TouchableObject* Create(Model* model = nullptr);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	bool Initialize(Model* model);
};