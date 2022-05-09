#pragma once

#include "SpriteCommon.h"
#include "Sprite.h"

// �f�o�b�O������N���X�̒�`
class Text
{
public: // �萔�̐錾    
	static const int maxCharCount = 256;    // �ő啶����
	static const int fontWidth = 9;         // �t�H���g�摜��1�������̉���
	static const int fontHeight = 18;       // �t�H���g�摜��1�������̏c��
	static const int fontLineCount = 14;    // �t�H���g�摜��1�s���̕�����
	static const int bufferSize = 256;	// �����t��������W�J�p�o�b�t�@�T�C�Y
public: // �����o�֐�
	Text();

	Text* GetInstance();

	void Initialize(SpriteCommon* sprCommon, UINT texnumber);

	void Print(const std::string& text, float x, float y, float scale = 1.0f);

	void Printf(const char* format, ...);

	void NPrint(int len, const char* text);

	void DrawAll(ID3D12GraphicsCommandList* cmdList);

	void SetPosition(float x, float y) {
		this->posX = x;
		this->posY = y;
	}

	void SetSize(float size) { this->size = size; }

private: // �����o�ϐ�
	// �X�v���C�g����
	SpriteCommon* spriteCommon = nullptr;
	// �X�v���C�g�f�[�^�̔z��
	Sprite* sprites[maxCharCount]{};
	// �X�v���C�g�f�[�^�z��̓Y�����ԍ�
	int spriteIndex = 0;

	float posX = 0.0f;
	float posY = 0.0f;
	float size = 1.0f;

	char buffer[bufferSize];
};