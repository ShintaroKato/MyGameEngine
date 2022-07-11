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
	// ���[���h�s��̍X�V
	matWorld = XMMatrixIdentity();
	// Z����]
	matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	// ���s�ړ�
	matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);

	TransferVertexBuffer();

	// �萔�o�b�t�@�ւ̓]��
	ConstBufferData* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = matWorld * this->spriteCommon->GetMatProjection();
	constMap->color = color;
	constBuff->Unmap(0, nullptr);

	cmdList = spriteCommon->GetCommandList();

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(spriteCommon->GetPipelineSet().pipelinestate.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(spriteCommon->GetPipelineSet().rootsignature.Get());
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	// �萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	// �V�F�[�_���\�[�X�r���[���Z�b�g
	spriteCommon->SetGraphicsRootDescriptorTable(1, texNumber);

	// �|���S���̕`��i4���_�Ŏl�p�`�j
	cmdList->DrawInstanced(4, 1, 0, 0);
}
