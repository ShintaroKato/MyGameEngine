#pragma once
#include "SceneBase.h"
#include "SceneManager.h"

/// <summary>
/// �Q�[���V�[��
/// </summary>
class SceneInGame : public SceneBase
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

private: // �ÓI�����o�ϐ�
	static const int debugTextTexNumber = 0;

public: // �����o�֐�

	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	SceneInGame();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~SceneInGame();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(DirectXCommon* dxCommon, SpriteCommon* sprCommon, Input* input, Audio* audio)override;

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update()override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw()override;

private: // �����o�ϐ�
	DirectXCommon* dxCommon = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	Text* text;

	/// <summary>
	/// �Q�[���V�[���p
	/// </summary>
	Camera* camera = nullptr;

	Sprite* spriteBG = nullptr;

	ModelOBJ* modelSphere = nullptr;
	ObjectOBJ* objSphere = nullptr;

	ModelFBX* fbxModelCube = nullptr;
	ObjectFBX* fbxCube = nullptr;
};