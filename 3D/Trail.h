#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>

#include "Camera.h"

/// <summary>
/// �O��
/// </summary>
class Trail
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
	// ���_�f�[�^�\����
	struct VertexPos
	{
		XMFLOAT3 pos; // xyz���W
		float scale; // �X�P�[��
		XMFLOAT4 color; //�J���[
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMMATRIX mat;	// �r���[�v���W�F�N�V�����s��
		XMMATRIX matBillboard;	// �r���{�[�h�s��
	};

	struct TrailInfo
	{
		XMFLOAT3 head;
		XMFLOAT3 tail;
	};

private: // �萔
	static const int vertexCount = 0xffff;		// ���_��

public: // �����o�֐�	
	/// <summary>
	/// ������
	/// </summary>
	/// <returns></returns>
	void Initialize(ID3D12Device* device);
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// ���f���̏�[�Ɖ��[��ݒ�
	/// </summary>
	void SetVertexPos(XMFLOAT3 head, XMFLOAT3 tail)
	{
		info.head = head;
		info.tail = tail;
	}

	/// <summary>
	/// �J�����̃Z�b�g
	/// </summary>
	/// <param name="camera">�J����</param>
	inline void SetCamera(Camera* camera) { this->camera = camera; }

	/// <summary>
	/// �f�X�N���v�^�q�[�v�̏�����
	/// </summary>
	/// <returns></returns>
	void InitializeDescriptorHeap();

	/// <summary>
	/// �O���t�B�b�N�p�C�v���C������
	/// </summary>
	/// <returns>����</returns>
	void InitializeGraphicsPipeline();

	/// <summary>
	/// ���f���쐬
	/// </summary>
	void CreateModel();

private: // �����o�ϐ�
	// �f�o�C�X
	ID3D12Device* device = nullptr;
	// �f�X�N���v�^�T�C�Y
	UINT descriptorHandleIncrementSize = 0u;
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	ComPtr<ID3D12PipelineState> pipelinestate;
	// �f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texbuff;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(GPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	// �J����
	Camera* camera = nullptr;
	TrailInfo info;
};