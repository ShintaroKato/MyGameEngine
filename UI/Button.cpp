#include "Button.h"
#include "Text.h"

Button::Button()
{
}

Button* Button::Create(SpriteCommon* spriteCommon, UINT texNumber, XMFLOAT2 position, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	Button* instance = new Button();

	instance->Initialize(spriteCommon, texNumber, position, anchorpoint, isFlipX, isFlipY);

	return instance;
}

bool Button::Click(MouseButton mouseButton)
{
	Input* input = Input::GetInstance();

	XMFLOAT2 mouse = input->GetMousePos2();

	if (input->TriggerMouse(mouseButton) &&
		mouse.x <= position.x + size.x * (1 - anchorpoint.x) && mouse.x >= position.x - size.x * anchorpoint.x &&
		mouse.y <= position.y + size.y * (1 - anchorpoint.y) && mouse.y >= position.y - size.y * anchorpoint.y)
	{
		click = true;
	}
	else
	{
		click = false;
	}

	return click;
}
