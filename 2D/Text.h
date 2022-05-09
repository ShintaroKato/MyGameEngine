#pragma once

#include "SpriteCommon.h"
#include "Sprite.h"

// デバッグ文字列クラスの定義
class Text
{
public: // 定数の宣言    
	static const int maxCharCount = 256;    // 最大文字数
	static const int fontWidth = 9;         // フォント画像内1文字分の横幅
	static const int fontHeight = 18;       // フォント画像内1文字分の縦幅
	static const int fontLineCount = 14;    // フォント画像内1行分の文字数
	static const int bufferSize = 256;	// 書式付き文字列展開用バッファサイズ
public: // メンバ関数
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

private: // メンバ変数
	// スプライト共通
	SpriteCommon* spriteCommon = nullptr;
	// スプライトデータの配列
	Sprite* sprites[maxCharCount]{};
	// スプライトデータ配列の添え字番号
	int spriteIndex = 0;

	float posX = 0.0f;
	float posY = 0.0f;
	float size = 1.0f;

	char buffer[bufferSize];
};