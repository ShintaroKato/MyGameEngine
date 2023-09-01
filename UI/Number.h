#pragma once
#include "Sprite.h"

class Number : public Sprite
{
public:

	Number();

	static Number* Create(SpriteCommon* spriteCommon, UINT texNumber, unsigned int digit, XMFLOAT2 position, XMFLOAT2 anchorpoint,
		bool isFlipX = false, bool isFlipY = false);

	/// <summary>
	/// �����\��
	/// </summary>
	/// <param name="value">����</param>
	/// <param name="x">X���W</param>
	/// <param name="y">Y���W</param>
	/// <param name="space">�������m�̊Ԋu</param>
	/// <param name="rectSize">�؂���T�C�Y</param>
	void SetValue(int value, float x, float y, XMFLOAT2 rectSize, int space = 0);

	void SetSize(const XMFLOAT2& size)
	{
		for (int i = 0; i < numbers.size(); i++)
		{
			numbers[i]->SetSize(size);
		}
	}

	/// <summary>
	/// �����ݒ�
	/// </summary>
	void SetDigit(unsigned int digit);

	void Draw();

private:
	// ����
	unsigned int digit = 1;
	// �؂蕪����ꂽ�摜��ۑ�����z��
	std::vector<Sprite*> numbers;
};