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
	//�f�o�C�X
	ComPtr<ID3D12Device> dev;
	ComPtr<IDXGIFactory6> dxgiFactory;

	//�R�}���h
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	ComPtr<ID3D12CommandQueue> cmdQueue;

	//�X���b�v�`�F�[��
	ComPtr<IDXGISwapChain4> swapchain;

	//�����_�[�^�[�Q�b�g�r���[
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;

	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuffer;

	// ���\�̂Q���ɂ���
	std::vector<ComPtr<ID3D12Resource>> backBuffers;

	ComPtr<ID3D12DescriptorHeap> dsvHeap;

	// �t�F���X
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

	void PreDraw();		//�`��O����
	void PostDraw();	//�`��㏈��

	ID3D12Device* GetDev() { return dev.Get(); }
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList.Get(); }
};