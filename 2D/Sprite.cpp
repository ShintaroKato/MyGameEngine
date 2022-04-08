#include "Sprite.h"

// �X�v���C�g���ʃO���t�B�b�N�R�}���h�̃Z�b�g
void SpriteCommonBeginDraw()
{
	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(spriteCommon.pipelineSet.pipelinestate.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(spriteCommon.pipelineSet.rootsignature.Get());
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �e�N�X�`���p�f�X�N���v�^�q�[�v�̐ݒ�
	ID3D12DescriptorHeap* ppHeaps[] = { spriteCommon.descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}


// �X�v���C�g�P�̍X�V
void SpriteUpdate(Sprite& sprite, const SpriteCommon& spriteCommon)
{
	// ���[���h�s��̍X�V
	sprite.matWorld = XMMatrixIdentity();
	// Z����]
	sprite.matWorld *= XMMatrixRotationZ(XMConvertToRadians(sprite.rotation));
	// ���s�ړ�
	sprite.matWorld *= XMMatrixTranslation(sprite.position.x, sprite.position.y, sprite.position.z);

	// �萔�o�b�t�@�̓]��
	ConstBufferData* constMap = nullptr;
	HRESULT result = sprite.constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = sprite.matWorld * spriteCommon.matProjection;
	constMap->color = sprite.color;
	sprite.constBuff->Unmap(0, nullptr);
}

// �X�v���C�g�P�̕`��
void SpriteDraw(ID3D12GraphicsCommandList* cmdList, ID3D12Device* dev)
{
	if (sprite.isInvisible) {
		return;
	}

	// ���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &sprite.vbView);

	// �萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, sprite.constBuff->GetGPUVirtualAddress());

	// �V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			spriteCommon.descHeap->GetGPUDescriptorHandleForHeapStart(),
			sprite.texNumber,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	// �|���S���̕`��i4���_�Ŏl�p�`�j
	cmdList->DrawInstanced(4, 1, 0, 0);
}