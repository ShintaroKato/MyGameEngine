#pragma once
#include "SceneBase.h"
#include "SceneManager.h"
#include "Player.h"
#include "GameObject.h"

/// <summary>
/// ゲームシーン
/// </summary>
class SceneStageEdit : public SceneBase
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

private: // 定数
	static const int debugTextTexNumber = 0;

	static const int CUBE_RED_MAX = 3;
	static const int CUBE_GREEN_MAX = 3;
	static const int CUBE_BLUE_MAX = 3;

public: // メンバ関数

	/// <summary>
	/// コンストクラタ
	/// </summary>
	SceneStageEdit();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneStageEdit();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxCommon, SpriteCommon* sprCommon, Input* input, Audio* audio)override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

private: // メンバ変数
	/// <summary>
	/// ゲームシーン用
	/// </summary>

	Text* text;

	Sprite* spriteBG = nullptr;

	ModelOBJ* modelSkydome = nullptr;
	ModelOBJ* modelGround = nullptr;
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