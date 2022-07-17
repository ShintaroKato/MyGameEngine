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
};
