#pragma once
#include "Sprite.h"
#include "Input.h"

class Button : public Sprite
{
protected: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	// コンストラクタ
	Button();

	static Button* Create(SpriteCommon* spriteCommon, UINT texNumber, XMFLOAT2 position, XMFLOAT2 anchorpoint,
		bool isFlipX = false, bool isFlipY = false);

	bool Click(MouseButton mouseButton);

	void SetClickFlag(bool flag) { click = flag; }

private:
	bool click = false;
};