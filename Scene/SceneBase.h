#pragma once
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Audio.h"
#include "Input.h"
#include "Sprite.h"
#include "Text.h"
#include "Object3d.h"
#include "ObjectFBX.h"
//#include "SceneManager.h"

class SceneBase
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(DirectXCommon* dxCommon, SpriteCommon* spriteCommon, Input* input, Audio* audio);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() {};

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() {};

private:

	DirectXCommon* dxCommon_ = nullptr;
	SpriteCommon* spriteCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
};