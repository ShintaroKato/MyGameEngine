#pragma once

#include <DirectXMath.h>
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
	// デバイス
	static ID3D12Device* device;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootSignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelineState;
	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// テクスチャバッファ
	static ComPtr<ID3D12Resource> texBuff[spriteSRVCount];
private:
	// パイプラインセット
	PipelineSet pipelineSet;
	// 射影行列
	XMMATRIX matProjection{};
	// テクスチャ用デスクリプタヒープの生成
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// テクスチャリソース（テクスチャバッファ）の配列
	ComPtr<ID3D12Resource> texBuff[spriteSRVCount];

public:
	void Initialize(ID3D12Device* dev);

	// スプライト用パイプライン生成
	void CreateGraphicsPipeline(ID3D12Device* dev);

	// スプライト共通テクスチャ読み込み
	void SpriteCommonLoadTexture(UINT texnumber, const wchar_t* filename);
};