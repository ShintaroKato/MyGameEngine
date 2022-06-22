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
	/// ������
	/// </summary>
	virtual void Initialize(DirectXCommon* dxCommon, SpriteCommon* spriteCommon, Input* input, Audio* audio);

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() {};

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw() {};

private:

	DirectXCommon* dxCommon_ = nullptr;
	SpriteCommon* spriteCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
};