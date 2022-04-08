#pragma once

#include <DirectXMath.h>
#include "PipelineSet.h"

// �X�v���C�g���ʕ�
class SpriteCommon
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
	// �e�N�X�`���̍ő喇��
	static const int spriteSRVCount = 512;
	// ���_��
	static const int vertNum = 4;
	// �f�o�C�X
	static ID3D12Device* device;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootSignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelineState;
	// �f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// �e�N�X�`���o�b�t�@
	static ComPtr<ID3D12Resource> texBuff[spriteSRVCount];
private:
	// �p�C�v���C���Z�b�g
	PipelineSet pipelineSet;
	// �ˉe�s��
	XMMATRIX matProjection{};
	// �e�N�X�`���p�f�X�N���v�^�q�[�v�̐���
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// �e�N�X�`�����\�[�X�i�e�N�X�`���o�b�t�@�j�̔z��
	ComPtr<ID3D12Resource> texBuff[spriteSRVCount];

public:
	void Initialize(ID3D12Device* dev);

	// �X�v���C�g�p�p�C�v���C������
	void CreateGraphicsPipeline(ID3D12Device* dev);

	// �X�v���C�g���ʃe�N�X�`���ǂݍ���
	void SpriteCommonLoadTexture(UINT texnumber, const wchar_t* filename);
};