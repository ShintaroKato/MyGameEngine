#pragma once
#include "Sprite.h"

class PostEffect : public Sprite
{
public:
	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMFLOAT4 color; // 色 (RGBA)
		XMMATRIX mat;   // ３Ｄ変換行列
		UINT mode;     // 表示切り替え
		float alpha;     // 透明度
	};

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

	void SwitchDraw();

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

	bool isDraw = true;

	XMFLOAT2 size = { 0.5f,0.5f };

	UINT mode = 2;
	float alpha = 1.0f;
};
