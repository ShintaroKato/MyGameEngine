#include "Sprite.h"
#include <DirectXTex.h>

using namespace DirectX;

Sprite::Sprite()
{

}

// スプライト生成
void Sprite::Initialize(SpriteCommon* spriteCommon, UINT texNumber, XMFLOAT2 position, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	HRESULT result = S_FALSE;

	this->spriteCommon = spriteCommon;
	// テクスチャ番号をコピー
	this->texNumber = texNumber;

	// アンカーポイントをコピー
	this->anchorpoint = anchorpoint;

	// 反転フラグをコピー
	this->isFlipX = isFlipX;
	this->isFlipY = isFlipY;

	// 頂点データ
	VertexPosUv vertices[4];

	// 指定番号の画像が読み込み済みなら
	if (this->spriteCommon->GetTexBuff(texNumber)) {
		// テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = this->spriteCommon->GetTexBuff(texNumber)->GetDesc();

		// スプライトの大きさを画像の解像度に合わせる
		size = { (float)resDesc.Width, (float)resDesc.Height };
		texSize = { (float)resDesc.Width, (float)resDesc.Height };
	}

	// 頂点バッファ生成
	result = this->spriteCommon->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff));

	// 頂点バッファデータ転送
	TransferVertexBuffer();

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);

	// 定数バッファの生成
	result = this->spriteCommon->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));

	// 定数バッファにデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1); // 色指定（RGBA）
	constMap->mat = spriteCommon->GetMatProjection();
	constBuff->Unmap(0, nullptr);
}

Sprite* Sprite::Create(SpriteCommon* spriteCommon, UINT texNumber, XMFLOAT2 position, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	Sprite* instance = new Sprite();

	instance->Initialize(spriteCommon, texNumber, position, anchorpoint, isFlipX, isFlipY);

	return instance;
}

void Sprite::Update()
{
	// ワールド行列の更新
	matWorld = XMMatrixIdentity();
	// Z軸回転
	matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	// 平行移動
	matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);

	TransferVertexBuffer();

	// 定数バッファへの転送
	ConstBufferData* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = matWorld * this->spriteCommon->GetMatProjection();
	constMap->color = color;
	constBuff->Unmap(0, nullptr);
}

// スプライト単体描画
void Sprite::Draw()
{
	if (isInvisible) {
		return;
	}

	cmdList = spriteCommon->GetCommandList();

	// 頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	// 定数バッファをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	// シェーダリソースビューをセット
	spriteCommon->SetGraphicsRootDescriptorTable(1, texNumber);

	// ポリゴンの描画（4頂点で四角形）
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void Sprite::SetRotation(float rotation)
{
	this->rotation = rotation;

	// 頂点バッファへデータ転送
	TransferVertexBuffer();
}

void Sprite::SetPosition(const XMFLOAT2& position)
{
	this->position = position;

	// 頂点バッファへデータ転送
	TransferVertexBuffer();
}

void Sprite::SetSize(const XMFLOAT2& size)
{
	this->size = size;

	// 頂点バッファへデータ転送
	TransferVertexBuffer();
}

void Sprite::SetAnchorPoint(const XMFLOAT2& anchorpoint)
{
	this->anchorpoint = anchorpoint;

	// 頂点バッファへデータ転送
	TransferVertexBuffer();
}

void Sprite::SetIsFlipX(bool isFlipX)
{
	this->isFlipX = isFlipX;

	// 頂点バッファへデータ転送
	TransferVertexBuffer();
}

void Sprite::SetIsFlipY(bool isFlipY)
{
	this->isFlipY = isFlipY;

	// 頂点バッファへデータ転送
	TransferVertexBuffer();
}

void Sprite::SetTextureRect(XMFLOAT2 texLeftTop, XMFLOAT2 texSize)
{
	this->texLeftTop = texLeftTop;
	this->texSize = texSize;

	// 頂点バッファへデータ転送
	TransferVertexBuffer();
}

// スプライト単体頂点バッファの転送
void Sprite::TransferVertexBuffer()
{
	HRESULT result = S_FALSE;

	// 頂点データ
	VertexPosUv vertices[] = {
		//     u     v
		{{}, {0.0f, 1.0f}}, // 左下
		{{}, {0.0f, 0.0f}}, // 左上
		{{}, {1.0f, 1.0f}}, // 右下
		{{}, {1.0f, 0.0f}}, // 右上
	};

	// 左下、左上、右下、右上
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorpoint.x) * size.x;
	float right = (1.0f - anchorpoint.x) * size.x;
	float top = (0.0f - anchorpoint.y) * size.y;
	float bottom = (1.0f - anchorpoint.y) * size.y;

	if (isFlipX)
	{// 左右入れ替え
		left = -left;
		right = -right;
	}

	if (isFlipY)
	{// 左右入れ替え
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left, bottom,  0.0f }; // 左下
	vertices[LT].pos = { left, top,     0.0f }; // 左上
	vertices[RB].pos = { right, bottom, 0.0f }; // 右下
	vertices[RT].pos = { right, top,    0.0f }; // 右上

	// 指定番号の画像が読み込み済みなら
	if (spriteCommon->GetTexBuff(texNumber)) {
		// テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = spriteCommon->GetTexBuff(texNumber)->GetDesc();

		float tex_left = texLeftTop.x / resDesc.Width;
		float tex_right = (texLeftTop.x + texSize.x) / resDesc.Width;
		float tex_top = texLeftTop.y / resDesc.Height;
		float tex_bottom = (texLeftTop.y + texSize.y) / resDesc.Height;

		vertices[LB].uv = { tex_left,   tex_bottom }; // 左下
		vertices[LT].uv = { tex_left,   tex_top }; // 左上
		vertices[RB].uv = { tex_right,  tex_bottom }; // 右下
		vertices[RT].uv = { tex_right,  tex_top }; // 右上
	}

	// 頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	vertBuff->Unmap(0, nullptr);
}