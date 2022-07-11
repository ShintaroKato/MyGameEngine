#include "Sprite.h"
#include <DirectXTex.h>

using namespace DirectX;

Sprite::Sprite()
{

}

// �X�v���C�g����
void Sprite::Initialize(SpriteCommon* spriteCommon, UINT texNumber, XMFLOAT2 position, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	HRESULT result = S_FALSE;

	this->spriteCommon = spriteCommon;
	// �e�N�X�`���ԍ����R�s�[
	this->texNumber = texNumber;

	// �A���J�[�|�C���g���R�s�[
	this->anchorpoint = anchorpoint;

	// ���]�t���O���R�s�[
	this->isFlipX = isFlipX;
	this->isFlipY = isFlipY;

	// ���_�f�[�^
	VertexPosUv vertices[4];

	// �w��ԍ��̉摜���ǂݍ��ݍς݂Ȃ�
	if (this->spriteCommon->GetTexBuff(texNumber)) {
		// �e�N�X�`�����擾
		D3D12_RESOURCE_DESC resDesc = this->spriteCommon->GetTexBuff(texNumber)->GetDesc();

		// �X�v���C�g�̑傫�����摜�̉𑜓x�ɍ��킹��
		size = { (float)resDesc.Width, (float)resDesc.Height };
		texSize = { (float)resDesc.Width, (float)resDesc.Height };
	}

	// ���_�o�b�t�@����
	result = this->spriteCommon->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff));

	// ���_�o�b�t�@�f�[�^�]��
	TransferVertexBuffer();

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);

	// �萔�o�b�t�@�̐���
	result = this->spriteCommon->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));

	// �萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1); // �F�w��iRGBA�j
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
}

// �X�v���C�g�P�̕`��
void Sprite::Draw()
{
	if (isInvisible) {
		return;
	}

	cmdList = spriteCommon->GetCommandList();

	// ���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	// �萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	// �V�F�[�_���\�[�X�r���[���Z�b�g
	spriteCommon->SetGraphicsRootDescriptorTable(1, texNumber);

	// �|���S���̕`��i4���_�Ŏl�p�`�j
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void Sprite::SetRotation(float rotation)
{
	this->rotation = rotation;

	// ���_�o�b�t�@�փf�[�^�]��
	TransferVertexBuffer();
}

void Sprite::SetPosition(const XMFLOAT2& position)
{
	this->position = position;

	// ���_�o�b�t�@�փf�[�^�]��
	TransferVertexBuffer();
}

void Sprite::SetSize(const XMFLOAT2& size)
{
	this->size = size;

	// ���_�o�b�t�@�փf�[�^�]��
	TransferVertexBuffer();
}

void Sprite::SetAnchorPoint(const XMFLOAT2& anchorpoint)
{
	this->anchorpoint = anchorpoint;

	// ���_�o�b�t�@�փf�[�^�]��
	TransferVertexBuffer();
}

void Sprite::SetIsFlipX(bool isFlipX)
{
	this->isFlipX = isFlipX;

	// ���_�o�b�t�@�փf�[�^�]��
	TransferVertexBuffer();
}

void Sprite::SetIsFlipY(bool isFlipY)
{
	this->isFlipY = isFlipY;

	// ���_�o�b�t�@�փf�[�^�]��
	TransferVertexBuffer();
}

void Sprite::SetTextureRect(XMFLOAT2 texLeftTop, XMFLOAT2 texSize)
{
	this->texLeftTop = texLeftTop;
	this->texSize = texSize;

	// ���_�o�b�t�@�փf�[�^�]��
	TransferVertexBuffer();
}

// �X�v���C�g�P�̒��_�o�b�t�@�̓]��
void Sprite::TransferVertexBuffer()
{
	HRESULT result = S_FALSE;

	// ���_�f�[�^
	VertexPosUv vertices[] = {
		//     u     v
		{{}, {0.0f, 1.0f}}, // ����
		{{}, {0.0f, 0.0f}}, // ����
		{{}, {1.0f, 1.0f}}, // �E��
		{{}, {1.0f, 0.0f}}, // �E��
	};

	// �����A����A�E���A�E��
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorpoint.x) * size.x;
	float right = (1.0f - anchorpoint.x) * size.x;
	float top = (0.0f - anchorpoint.y) * size.y;
	float bottom = (1.0f - anchorpoint.y) * size.y;

	if (isFlipX)
	{// ���E����ւ�
		left = -left;
		right = -right;
	}

	if (isFlipY)
	{// ���E����ւ�
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left, bottom,  0.0f }; // ����
	vertices[LT].pos = { left, top,     0.0f }; // ����
	vertices[RB].pos = { right, bottom, 0.0f }; // �E��
	vertices[RT].pos = { right, top,    0.0f }; // �E��

	// �w��ԍ��̉摜���ǂݍ��ݍς݂Ȃ�
	if (spriteCommon->GetTexBuff(texNumber)) {
		// �e�N�X�`�����擾
		D3D12_RESOURCE_DESC resDesc = spriteCommon->GetTexBuff(texNumber)->GetDesc();

		float tex_left = texLeftTop.x / resDesc.Width;
		float tex_right = (texLeftTop.x + texSize.x) / resDesc.Width;
		float tex_top = texLeftTop.y / resDesc.Height;
		float tex_bottom = (texLeftTop.y + texSize.y) / resDesc.Height;

		vertices[LB].uv = { tex_left,   tex_bottom }; // ����
		vertices[LT].uv = { tex_left,   tex_top }; // ����
		vertices[RB].uv = { tex_right,  tex_bottom }; // �E��
		vertices[RT].uv = { tex_right,  tex_top }; // �E��
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	vertBuff->Unmap(0, nullptr);
}