#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include "SpriteCommon.h"

// �X�v���C�g
class Sprite
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	// �ˉe�s��
	static XMMATRIX matProjection;

private:
	//���_�o�b�t�@;
	ComPtr<ID3D12Resource> vertBuff;
	//���_�o�b�t�@�r���[;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�萔�o�b�t�@;
	ComPtr<ID3D12Resource> constBuff;
	// Z�����̉�]�p
	float rotation = 0.0f;
	// ���W
	XMFLOAT3 position = { 0,0,0 };
	// ���[���h�s��
	XMMATRIX matWorld;
	// �F(RGBA)
	XMFLOAT4 color = { 1, 1, 1, 1 };
	// �e�N�X�`���ԍ�
	UINT texNumber = 0;
	// �傫��
	XMFLOAT2 size = { 100, 100 };
	// �A���J�[�|�C���g
	XMFLOAT2 anchorpoint = { 0.5f, 0.5f };
	// ���E���]
	bool isFlipX = false;
	// �㉺���]
	bool isFlipY = false;
	// �e�N�X�`��������W
	XMFLOAT2 texLeftTop = { 0, 0 };
	// �e�N�X�`���؂�o���T�C�Y
	XMFLOAT2 texSize = { 100, 100 };
	// ��\��
	bool isInvisible = false;
};