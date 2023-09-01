#pragma once
#include "SceneBase.h"
#include "GameManager.h"

/// <summary>
/// ゲームシーン
/// </summary>
class SceneInGame : public SceneBase
{
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

	/// <summary>
	/// メニュー表示
	/// </summary>
	void UpdateMenu();

	void DrawMenu();

	/// <summary>
	/// ナビメッシュ再生成
	/// </summary>
	void RecreateNaviMesh();

private:
	bool windowActive = false;
	bool menuActive = false;
};