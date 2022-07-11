#pragma once
#include "Sprite.h"

class PostEffect : public Sprite
{
public:
	PostEffect();

	static PostEffect* Create(SpriteCommon* spriteCommon, UINT texNumber, XMFLOAT2 position,
		XMFLOAT2 anchorpoint = { 0.5f,0.5f }, bool isFlipX = false, bool isFlipY = false);

	void Draw();

private:
	PipelineSet pipelineSet;
};

