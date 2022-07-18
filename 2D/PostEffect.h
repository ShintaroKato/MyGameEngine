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
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff;
	// 深度バッファ
	ComPtr<ID3D12Resource> depthBuff;
	// デスクリプタヒープ(SRV用)
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	// デスクリプタヒープ(RTV用)
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	// デスクリプタヒープ(DSV用)
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	// パイプラインセット
	PipelineSet pipelineSet;

public:
	XMFLOAT2 size = { 0.5f,0.5f };
};
