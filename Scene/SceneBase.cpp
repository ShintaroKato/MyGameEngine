#include "SceneBase.h"

void SceneBase::Initialize(DirectXCommon* dxCommon, SpriteCommon* spriteCommon, Input* input, Audio* audio)
{
	// nullptr�`�F�b�N
	assert(dxCommon);
	assert(spriteCommon);
	assert(input);

	this->dxCommon_ = dxCommon;
	this->spriteCommon_ = spriteCommon;
	this->input_ = input;
	this->audio_ = audio;
}