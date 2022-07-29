#include "Text.h"

Text::Text()
{
}

Text* Text::GetInstance()
{
	static Text instance;

	return &instance;
}

void Text::Initialize(SpriteCommon* sprCommon, UINT texnumber)
{
	this->spriteCommon = sprCommon;

	// �S�ẴX�v���C�g�f�[�^�ɂ���
	for (int i = 0; i < _countof(sprites); i++)
	{
		// �X�v���C�g�𐶐�����
		sprites[i] = Sprite::Create(spriteCommon, texnumber, { 0,0 });
	}
}

void Text::Print(const std::string& text, float x, float y, float scale)
{
	SetPosition(x, y);
	SetSize(scale);
}

void Text::Printf(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	int w = vsnprintf(buffer, bufferSize - 1, format, args);
	NPrint(w, buffer);
	va_end(args);
}

void Text::NPrint(int len, const char* text)
{
	// �S�Ă̕����ɂ���
	for (int i = 0; i < len; i++)
	{
		// �ő啶��������
		if (spriteIndex >= maxCharCount) {
			break;
		}

		// 1�������o��(��ASCII�R�[�h�ł������藧���Ȃ�)
		const unsigned char& character = text[i];

		// ASCII�R�[�h��2�i����΂����ԍ����v�Z
		int fontIndex = character - 32;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		// ���W�v�Z
		sprites[spriteIndex]->SetPosition({ posX + fontWidth * size * i, posY });
		sprites[spriteIndex]->SetTextureRect(
			{ (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight },
			{ fontWidth, fontHeight });
		sprites[spriteIndex]->SetSize({ fontWidth * size, fontHeight * size });
		// �X�V
		sprites[spriteIndex]->Update();

		// �������P�i�߂�
		spriteIndex++;
	}
}

// �܂Ƃ߂ĕ`��
void Text::DrawAll()
{
	// �S�Ă̕����̃X�v���C�g�ɂ���
	for (int i = 0; i < spriteIndex; i++)
	{
		// �X�v���C�g�`��
		sprites[i]->Draw();
	}

	spriteIndex = 0;
}