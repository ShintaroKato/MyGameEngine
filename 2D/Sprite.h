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
protected: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
	// ���_�f�[�^
	struct VertexPosUv
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT2 uv;  // uv���W
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMFLOAT4 color; // �F (RGBA)
		XMMATRIX mat;   // �R�c�ϊ��s��
	};

public:
	// �R���X�g���N�^
	Sprite();

	static Sprite* Create(SpriteCommon* spriteCommon, UINT texNumber, XMFLOAT2 position,
		XMFLOAT2 anchorpoint = { 0.5f,0.5f }, bool isFlipX = false, bool isFlipY = false);

	void Initialize(SpriteCommon* spriteCommon, UINT texNumber, XMFLOAT2 position,
		XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);

	// �X�v���C�g�P�̍X�V
	void Update();

	// �X�v���C�g�P�̕`��
	void Draw();

	/// <summary>
	/// �p�x�̐ݒ�
	/// </summary>
	/// <param name="rotation">�p�x</param>
	void SetRotation(float rotation);

	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(const XMFLOAT2& position);

	/// <summary>
	/// �T�C�Y�̐ݒ�
	/// </summary>
	/// <param name="size">�T�C�Y</param>
	void SetSize(const XMFLOAT2& size);

	/// <summary>
	/// �A���J�[�|�C���g�̐ݒ�
	/// </summary>
	/// <param name="anchorpoint">�A���J�[�|�C���g</param>
	void SetAnchorPoint(const XMFLOAT2& anchorpoint);

	/// <summary>
	/// ���E���]�̐ݒ�
	/// </summary>
	/// <param name="isFlipX">���E���]</param>
	void SetIsFlipX(bool isFlipX);

	/// <summary>
	/// �㉺���]�̐ݒ�
	/// </summary>
	/// <param name="isFlipX">�㉺���]</param>
	void SetIsFlipY(bool isFlipY);

	/// <summary>
	/// �e�N�X�`���͈͐ݒ�
	/// </summary>
	/// <param name="texBase">�e�N�X�`��������W</param>
	/// <param name="texSize">�e�N�X�`���T�C�Y</param>
	void SetTextureRect(XMFLOAT2 texLeftTop, XMFLOAT2 texSize);

protected: // �����o�֐�
	/// <summary>
	/// ���_�f�[�^�]��
	/// </summary>
	void TransferVertexBuffer();

protected:
	// �X�v���C�g���ʕ���
	SpriteCommon* spriteCommon = nullptr;
	// �R�}���h���X�g
	ID3D12GraphicsCommandList* cmdList = nullptr;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// �e�N�X�`���ԍ�
	UINT texNumber = 0;
	// Z�����̉�]�p
	float rotation = 0.0f;
	// ���W
	XMFLOAT2 position{};
	// �X�v���C�g���A����
	XMFLOAT2 size = { 100.0f, 100.0f };
	// �A���J�[�|�C���g
	XMFLOAT2 anchorpoint = { 0, 0 };
	// ���[���h�s��
	XMMATRIX matWorld{};
	// �F
	XMFLOAT4 color = { 1, 1, 1, 1 };
	// ���E���]
	bool isFlipX = false;
	// �㉺���]
	bool isFlipY = false;
	// �e�N�X�`���n�_
	XMFLOAT2 texLeftTop = { 0, 0 };
	// �e�N�X�`�����A����
	XMFLOAT2 texSize = { 100.0f, 100.0f };
	// ��\��
	bool isInvisible = false;

};