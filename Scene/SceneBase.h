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
protected: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

protected: // 定数
	static const int debugTextTexNumber = 0;

	static const int CUBE_RED_MAX = 10;
	static const int CUBE_GREEN_MAX = 10;
	static const int CUBE_BLUE_MAX = 10;

public:
	static XMFLOAT3 tmp[OBJECT_MAX];

public:
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(DirectXCommon* dxCommon, SpriteCommon* spriteCommon, Input* input, Audio* audio);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() {};

protected:

	DirectXCommon* dxCommon = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;

	Camera* camera = nullptr;

	/// <summary>
	/// ゲームシーン用
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