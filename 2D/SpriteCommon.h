#pragma once

#include <DirectXMath.h>
#include <d3dx12.h>
#include "PipelineSet.h"

// スプライト共通部
class SpriteCommon
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	// テクスチャの最大枚数
	static const int spriteSRVCount = 512;
	// 頂点数
	static const int vertNum = 4;

private:
	// デバイス
	static ID3D12Device* device;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// テクスチャバッファ
	static ComPtr<ID3D12Resource> texBuff[spriteSRVCount];
	// パイプラインセット
	static PipelineSet pipelineSet;
	// 射影行列
	static XMMATRIX matProjection;
	// パス
	static const std::string baseDirectory;

public:
	void Initialize(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height);

	// スプライト共通テクスチャ読み込み
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
	// スプライト用パイプライン生成
	void CreateGraphicsPipeline();
};