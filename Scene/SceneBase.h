#pragma once
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Audio.h"
#include "Input.h"
#include "Sprite.h"
#include "Text.h"
#include "ObjectOBJ.h"
#include "ObjectFBX.h"
#include "Player.h"
#include "GameObject.h"
#define OBJECT_MAX 30

class SceneBase
{
protected: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

protected: // �萔
	static const int debugTextTexNumber = 0;

	static const int CUBE_RED_MAX = 10;
	static const int CUBE_GREEN_MAX = 10;
	static const int CUBE_BLUE_MAX = 10;

public:
	static XMFLOAT3 tmp[OBJECT_MAX];

public:
	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize(DirectXCommon* dxCommon, SpriteCommon* spriteCommon, Input* input, Audio* audio);

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update();

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw() {};

protected:

	DirectXCommon* dxCommon = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;

	Camera* camera = nullptr;

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