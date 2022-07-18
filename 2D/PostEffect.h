#pragma once
#include "Sprite.h"

class PostEffect : public Sprite
{
public:
	PostEffect();

	static PostEffect* Create();

	void Initialize();

	void Draw();

	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

	void CreateGraphicsPipelineState();

	void TransferVertexBuffer();

	void SetTexSize(XMFLOAT2 size);

private:
	static const float clearColor[4];

private:
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff;
	// �[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuff;
	// �f�X�N���v�^�q�[�v(SRV�p)
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	// �f�X�N���v�^�q�[�v(RTV�p)
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	// �f�X�N���v�^�q�[�v(DSV�p)
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	// �p�C�v���C���Z�b�g
	PipelineSet pipelineSet;

public:
	XMFLOAT2 size = { 0.5f,0.5f };
};
