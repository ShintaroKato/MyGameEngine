#include "PostEffect.h"
#include <d3d12.h>

using namespace DirectX;

PostEffect::PostEffect() : Sprite()
{

}

PostEffect* PostEffect::Create(SpriteCommon* spriteCommon, UINT texNumber, XMFLOAT2 position, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{ 
	PostEffect* instance = new PostEffect();

	instance->Initialize(spriteCommon, texNumber, position, anchorpoint, isFlipX, isFlipY);

	return instance;
}

void PostEffect::Draw()
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

	cmdList = spriteCommon->GetCommandList();

	// パイプラインステートの設定
	cmdList->SetPipelineState(spriteCommon->GetPipelineSet().pipelinestate.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(spriteCommon->GetPipelineSet().rootsignature.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	// 定数バッファをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	// シェーダリソースビューをセット
	spriteCommon->SetGraphicsRootDescriptorTable(1, texNumber);

	// ポリゴンの描画（4頂点で四角形）
	cmdList->DrawInstanced(4, 1, 0, 0);
}
