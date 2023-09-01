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

void Button::Update()
{
	if (Point())
	{
		SetColor({ 1.2f, 1.2f, 1.2f, 1.0f });
	}
	else
	{
		SetColor({ 1,1,1,1 });
	}

	Sprite::Update();
}

bool Button::Click(MouseButton mouseButton)
{
	Input* input = Input::GetInstance();

	if (input->TriggerMouse(mouseButton) && point)
	{
		SetColor({ 0.5f, 0.5f, 0.5f, 1.0f });

		click = true;
	}
	else
	{
		click = false;
	}

	return click;
}

bool Button::Hold(MouseButton mouseButton)
{
	Input* input = Input::GetInstance();

	if (input->PushMouse(mouseButton) && point)
	{
		SetColor({ 0.5f, 0.5f, 0.5f, 1.0f });

		hold = true;
	}
	else
	{
		hold = false;
	}

	if (input->ReleaseMouse(mouseButton) && point)
	{
		hold = false;
		click = true;
	}

	return click;
}

bool Button::Point()
{
	Input* input = Input::GetInstance();

	XMFLOAT2 mouse = input->GetMousePos2();

	if (mouse.x <= position.x + size.x * (1 - anchorpoint.x) && mouse.x >= position.x - size.x * anchorpoint.x &&
		mouse.y <= position.y + size.y * (1 - anchorpoint.y) && mouse.y >= position.y - size.y * anchorpoint.y)
	{
		point = true;
	}
	else
	{
		point = false;
	}

	return point;
}
