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

	// 全てのスプライトデータについて
	for (int i = 0; i < _countof(sprites); i++)
	{
		// スプライトを生成する
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
	// 全ての文字について
	for (int i = 0; i < len; i++)
	{
		// 最大文字数超過
		if (spriteIndex >= maxCharCount) {
			break;
		}

		// 1文字取り出す(※ASCIIコードでしか成り立たない)
		const unsigned char& character = text[i];

		// ASCIIコードの2段分飛ばした番号を計算
		int fontIndex = character - 32;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		// 座標計算
		sprites[spriteIndex]->SetPosition({ posX + fontWidth * size * i, posY });
		sprites[spriteIndex]->SetTextureRect(
			{ (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight },
			{ fontWidth, fontHeight });
		sprites[spriteIndex]->SetSize({ fontWidth * size, fontHeight * size });
		// 更新
		sprites[spriteIndex]->Update();

		// 文字を１つ進める
		spriteIndex++;
	}
}

// まとめて描画
void Text::DrawAll()
{
	// 全ての文字のスプライトについて
	for (int i = 0; i < spriteIndex; i++)
	{
		// スプライト描画
		sprites[i]->Draw();
	}

	spriteIndex = 0;
}