#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include "SpriteCommon.h"

// スプライト
class Sprite
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
	// 射影行列
	static XMMATRIX matProjection;

private:
	//頂点バッファ;
	ComPtr<ID3D12Resource> vertBuff;
	//頂点バッファビュー;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//定数バッファ;
	ComPtr<ID3D12Resource> constBuff;
	// Z軸回りの回転角
	float rotation = 0.0f;
	// 座標
	XMFLOAT3 position = { 0,0,0 };
	// ワールド行列
	XMMATRIX matWorld;
	// 色(RGBA)
	XMFLOAT4 color = { 1, 1, 1, 1 };
	// テクスチャ番号
	UINT texNumber = 0;
	// 大きさ
	XMFLOAT2 size = { 100, 100 };
	// アンカーポイント
	XMFLOAT2 anchorpoint = { 0.5f, 0.5f };
	// 左右反転
	bool isFlipX = false;
	// 上下反転
	bool isFlipY = false;
	// テクスチャ左上座標
	XMFLOAT2 texLeftTop = { 0, 0 };
	// テクスチャ切り出しサイズ
	XMFLOAT2 texSize = { 100, 100 };
	// 非表示
	bool isInvisible = false;
};