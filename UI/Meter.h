#pragma once
#include "Sprite.h"

class Meter
{
public: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	Meter();

	static Meter* Create(SpriteCommon* spriteCommon,
		UINT texNumberBase, UINT texNumberInner, UINT texNumberFrame,
		XMFLOAT2 position, XMFLOAT2 anchorpoint = {0.0f, 0.5f}, bool isFlipX = false, bool isFlipY = false);

	void SetValue(float value, float valueMax)
	{
		this->value = value;
		this->valueMax = valueMax;
	}

	void SetSize(XMFLOAT2 baseSize, XMFLOAT2 innerSize, XMFLOAT2 frameSize);

	void SetPosition(XMFLOAT2 pos);

	void Update();

	void Draw();

private:
	Sprite* base = nullptr;
	Sprite* inner = nullptr;
	Sprite* frame = nullptr;

	XMFLOAT2 innerSize{};

	float value = 0;
	float valueMax = 0;
};