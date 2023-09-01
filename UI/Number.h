#pragma once
#include "Sprite.h"

class Number : public Sprite
{
public:

	Number();

	static Number* Create(SpriteCommon* spriteCommon, UINT texNumber, unsigned int digit, XMFLOAT2 position, XMFLOAT2 anchorpoint,
		bool isFlipX = false, bool isFlipY = false);

	/// <summary>
	/// 数列を表示
	/// </summary>
	/// <param name="value">数量</param>
	/// <param name="x">X座標</param>
	/// <param name="y">Y座標</param>
	/// <param name="space">文字同士の間隔</param>
	/// <param name="rectSize">切り取るサイズ</param>
	void SetValue(int value, float x, float y, XMFLOAT2 rectSize, int space = 0);

	void SetSize(const XMFLOAT2& size)
	{
		for (int i = 0; i < numbers.size(); i++)
		{
			numbers[i]->SetSize(size);
		}
	}

	/// <summary>
	/// 桁数設定
	/// </summary>
	void SetDigit(unsigned int digit);

	void Draw();

private:
	// 桁数
	unsigned int digit = 1;
	// 切り分けられた画像を保存する配列
	std::vector<Sprite*> numbers;
};