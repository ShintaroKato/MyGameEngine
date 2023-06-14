#include "Number.h"

Number::Number()
{
}

Number* Number::Create(SpriteCommon* spriteCommon, UINT texNumber, unsigned int digit_, XMFLOAT2 position, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	Number* instance = new Number();

	instance->Initialize(spriteCommon, texNumber, position, anchorpoint, isFlipX, isFlipY);

	// 桁数設定
	if (digit_ == 0)
	{
		digit_ = 1;
	}
	instance->digit = digit_;

	for (int i = 0; i < instance->digit; i++)
	{
		instance->numbers.push_back(Sprite::Create(spriteCommon, texNumber, position, anchorpoint, isFlipX, isFlipY));
	}

	return instance;
}

void Number::SetSequence(int value, float x, float y, XMFLOAT2 rectSize, int space)
{
	// 各位の数字を保存
	std::vector<char> eachNumber(digit);

	// 表示する数量
	int number = value;

	// 表示する数量の桁数と一致する10のn乗の数を求める
	int digit_ = pow(10, digit) / 10;

	for (int i = 0; i < eachNumber.size(); i++)
	{
		// 指定した数量を10^nで除算し、一番上の位から求める
		eachNumber[i] = number / digit_;
		number = number % digit_;
		digit_ = digit_ / 10;
	}

	for (int i = 0; i < eachNumber.size(); i++)
	{
		numbers[i]->SetSize(rectSize);
		numbers[i]->SetTextureRect({ (float)(rectSize.x * eachNumber[i]), 0 }, rectSize);
		numbers[i]->SetPosition({ (float)(rectSize.x * i) + x + space, y });
		numbers[i]->Update();
	}
}

void Number::SetDigit(unsigned int digit)
{
	this->digit = digit;

	numbers.clear();
	for (int i = 0; i < this->digit; i++)
	{
		numbers.push_back(Sprite::Create(spriteCommon, texNumber, position, anchorpoint, isFlipX, isFlipY));
	}
}

void Number::Draw()
{
	for (int i = 0; i < digit; i++)
	{
		numbers[i]->Draw();
	}
}