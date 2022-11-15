#include "Meter.h"

Meter::Meter()
{
}

Meter* Meter::Create(SpriteCommon* spriteCommon, UINT texNumberBase, UINT texNumberInner, UINT texNumberFrame, XMFLOAT2 position, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	Meter* instance = new Meter();

	instance->base = Sprite::Create(spriteCommon, texNumberBase, position, anchorpoint, isFlipX, isFlipY);
	instance->inner = Sprite::Create(spriteCommon, texNumberInner, position, anchorpoint, isFlipX, isFlipY);
	instance->frame = Sprite::Create(spriteCommon, texNumberFrame, position, anchorpoint, isFlipX, isFlipY);

	return instance;
}

void Meter::SetSize(XMFLOAT2 baseSize, XMFLOAT2 innerSize, XMFLOAT2 frameSize)
{
	base->SetSize(baseSize);
	inner->SetSize(innerSize);
	frame->SetSize(frameSize);

	this->innerSize = innerSize;

	Update();
}

void Meter::SetPosition(XMFLOAT2 pos)
{
	base->SetPosition(pos);
	inner->SetPosition(pos);
	frame->SetPosition(pos);

	Update();
}

void Meter::Update()
{
	float vRate = value / valueMax;

	inner->SetSize({ innerSize.x * vRate, innerSize.y * vRate });

	base->Update();
	inner->Update();
	frame->Update();
}

void Meter::Draw()
{
	base->Draw();
	inner->Draw();
	frame->Draw();
}
