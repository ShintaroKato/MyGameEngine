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
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 頂点データ
	struct VertexPosUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT2 uv;  // uv座標
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMFLOAT4 color; // 色 (RGBA)
		XMMATRIX mat;   // ３Ｄ変換行列
	};

public:
	// コンストラクタ
	Sprite();

	static Sprite* Create(SpriteCommon* spriteCommon, UINT texNumber, XMFLOAT2 position,
		XMFLOAT2 anchorpoint = { 0.5f,0.5f }, bool isFlipX = false, bool isFlipY = false);

	void Initialize(SpriteCommon* spriteCommon, UINT texNumber, XMFLOAT2 position,
		XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);

	// スプライト単体更新
	void Update();

	// スプライト単体描画
	void Draw();

	/// <summary>
	/// 角度の設定
	/// </summary>
	/// <param name="rotation">角度</param>
	void SetRotation(float rotation);

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const XMFLOAT2& position);

	/// <summary>
	/// サイズの設定
	/// </summary>
	/// <param name="size">サイズ</param>
	void SetSize(const XMFLOAT2& size);

	/// <summary>
	/// アンカーポイントの設定
	/// </summary>
	/// <param name="anchorpoint">アンカーポイント</param>
	void SetAnchorPoint(const XMFLOAT2& anchorpoint);

	/// <summary>
	/// 左右反転の設定
	/// </summary>
	/// <param name="isFlipX">左右反転</param>
	void SetIsFlipX(bool isFlipX);

	/// <summary>
	/// 上下反転の設定
	/// </summary>
	/// <param name="isFlipX">上下反転</param>
	void SetIsFlipY(bool isFlipY);

	/// <summary>
	/// テクスチャ範囲設定
	/// </summary>
	/// <param name="texBase">テクスチャ左上座標</param>
	/// <param name="texSize">テクスチャサイズ</param>
	void SetTextureRect(XMFLOAT2 texLeftTop, XMFLOAT2 texSize);

protected: // メンバ関数
	/// <summary>
	/// 頂点データ転送
	/// </summary>
	void TransferVertexBuffer();

protected:
	// スプライト共通部分
	SpriteCommon* spriteCommon = nullptr;
	// コマンドリスト
	ID3D12GraphicsCommandList* cmdList = nullptr;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// テクスチャ番号
	UINT texNumber = 0;
	// Z軸回りの回転角
	float rotation = 0.0f;
	// 座標
	XMFLOAT2 position{};
	// スプライト幅、高さ
	XMFLOAT2 size = { 100.0f, 100.0f };
	// アンカーポイント
	XMFLOAT2 anchorpoint = { 0, 0 };
	// ワールド行列
	XMMATRIX matWorld{};
	// 色
	XMFLOAT4 color = { 1, 1, 1, 1 };
	// 左右反転
	bool isFlipX = false;
	// 上下反転
	bool isFlipY = false;
	// テクスチャ始点
	XMFLOAT2 texLeftTop = { 0, 0 };
	// テクスチャ幅、高さ
	XMFLOAT2 texSize = { 100.0f, 100.0f };
	// 非表示
	bool isInvisible = false;

};