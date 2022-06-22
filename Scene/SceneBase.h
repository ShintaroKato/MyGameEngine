#pragma once
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Audio.h"
#include "Input.h"
#include "Sprite.h"
#include "Text.h"
#include "ObjectOBJ.h"
#include "ObjectFBX.h"

class SceneBase
{
public:
	/// <summary>
	/// èâä˙âª
	/// </summary>
	virtual void Initialize(DirectXCommon* dxCommon, SpriteCommon* spriteCommon, Input* input, Audio* audio);

	/// <summary>
	/// çXêV
	/// </summary>
	virtual void Update() {};

	/// <summary>
	/// ï`âÊ
	/// </summary>
	virtual void Draw() {};

private:

	DirectXCommon* dxCommon_ = nullptr;
	SpriteCommon* spriteCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
};