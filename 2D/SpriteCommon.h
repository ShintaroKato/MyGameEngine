#pragma once

#include <DirectXMath.h>
#include <d3dx12.h>
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

private:
	// �f�o�C�X
	static ID3D12Device* device;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// �f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// �e�N�X�`���o�b�t�@
	static ComPtr<ID3D12Resource> texBuff[spriteSRVCount];
	// �p�C�v���C���Z�b�g
	static PipelineSet pipelineSet;
	// �ˉe�s��
	static XMMATRIX matProjection;
	// �p�X
	static const std::string baseDirectory;

public:
	void Initialize(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height);

	// �X�v���C�g���ʃe�N�X�`���ǂݍ���
	void LoadTexture(UINT texnumber, const std::string& filename);

	void PreDraw(ID3D12GraphicsCommandList* cmdList);

	void PostDraw();

	ID3D12Resource* GetTexBuff(UINT texNumber);

	ID3D12Device* GetDevice() { return device; }

	const XMMATRIX GetMatProjection() { return matProjection; }

	ID3D12GraphicsCommandList* GetCommandList() { return cmdList; }

	void SetGraphicsRootDescriptorTable(UINT rootParamIndex, UINT texNumber);

	PipelineSet GetPipelineSet() { return pipelineSet; }

private:
	// �X�v���C�g�p�p�C�v���C������
	void CreateGraphicsPipeline();
};