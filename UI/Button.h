#pragma once
#include "Sprite.h"
#include "Input.h"

class Button : public Sprite
{
protected: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	// �R���X�g���N�^
	Button();

	static Button* Create(SpriteCommon* spriteCommon, UINT texNumber, XMFLOAT2 position, XMFLOAT2 anchorpoint,
		bool isFlipX = false, bool isFlipY = false);

	bool Click(MouseButton mouseButton);

	// �J�[�\�����{�^���Əd�Ȃ��Ă��邩�𔻒�
	bool Point();

	void SetClickFlag(bool flag) { click = flag; }

	bool GetPointFlag() { return point; }

private:
	bool click = false;
	bool point = false;
};