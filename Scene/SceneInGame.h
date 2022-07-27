#pragma once
#include "SceneBase.h"
#include "SceneManager.h"

/// <summary>
/// ゲームシーン
/// </summary>
class SceneInGame : public SceneBase
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

private: // 静的メンバ変数
	static const int debugTextTexNumber = 0;

public: // メンバ関数

	/// <summary>
	/// コンストクラタ
	/// </summary>
	SceneInGame();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneInGame();

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

	Physics* physics = nullptr;

	ModelOBJ* modelSphere = nullptr;
	ObjectOBJ* objSphere = nullptr;

	ModelOBJ* modelGround = nullptr;
	ObjectOBJ* objGround = nullptr;

	ModelFBX* fbxModelAnim = nullptr;
	ObjectFBX* fbxAnimTest = nullptr;
};