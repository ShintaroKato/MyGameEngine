#pragma once
#include "SceneBase.h"
#include "SceneManager.h"
#include "Player.h"
#include "GameObject.h"

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

private: // �萔
	static const int debugTextTexNumber = 0;

	static const int CUBE_RED_MAX = 10;
	static const int CUBE_GREEN_MAX = 10;
	static const int CUBE_BLUE_MAX = 10;

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

	/// <summary>
	/// dat�t�@�C���ǂݍ���
	/// </summary>
	void LoadDat();	

private: // �����o�ϐ�
	/// <summary>
	/// �Q�[���V�[���p
	/// </summary>

	Text* text;

	Sprite* spriteBG = nullptr;

	ModelOBJ* modelSkydome = nullptr;
	ModelOBJ* modelGround = nullptr;
	ModelOBJ* modelPlayer = nullptr;
	ModelOBJ* modelCubeRed = nullptr;
	ModelOBJ* modelCubeGreen = nullptr;
	ModelOBJ* modelCubeBlue = nullptr;

	ModelFBX* fbxModelAnim = nullptr;

	ObjectOBJ* objSkydome = nullptr;
	ObjectOBJ* objGround = nullptr;

	GameObject* objCubeRed[CUBE_RED_MAX]{};
	GameObject* objCubeGreen[CUBE_GREEN_MAX]{};
	GameObject* objCubeBlue[CUBE_BLUE_MAX]{};

	Player* player = nullptr;
};