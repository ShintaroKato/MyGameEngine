#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include <wrl.h>
#include "WinApp.h"

using namespace Microsoft::WRL;

class DirectXCommon
{
private:
	//デバイス
	ComPtr<ID3D12Device> dev;
	ComPtr<IDXGIFactory6> dxgiFactory;

	//コマンド
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	ComPtr<ID3D12CommandQueue> cmdQueue;

	//スワップチェーン
	ComPtr<IDXGISwapChain4> swapchain;

	//レンダーターゲットビュー
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;

	//深度バッファ
	ComPtr<ID3D12Resource> depthBuffer;

	// 裏表の２つ分について
	std::vector<ComPtr<ID3D12Resource>> backBuffers;

	ComPtr<ID3D12DescriptorHeap> dsvHeap;

	// フェンス
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	WinApp* winApp = nullptr;

public:
	void Initialize(WinApp* winApp);
	void InitializeDevice();
	void InitializeCommand();
	void InitializeSwapchain();
	void InitializeRenderTargetView();
	void InitializeDepthBuffer();
	void InitializeFence();

	void PreDraw();		//描画前処理
	void PostDraw();	//描画後処理

	ID3D12Device* GetDev() { return dev.Get(); }
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList.Get(); }
};