#pragma once
#include "SceneBase.h"

/// <summary>
/// ゲームシーン
/// </summary>
class SceneStageEdit : public SceneBase
{
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

	/// <summary>
	/// メニュー表示更新
	/// </summary>
	void MenuUpdate();

	/// <summary>
	/// メニュー表示描画
	/// </summary>
	void MenuDraw();

	/// <summary>
	/// オブジェクト作成
	/// </summary>
	bool MakeObject(StageObject* stgObject, Button* button, ModelOBJ* model, const Tag& objectTag, const XMFLOAT2& sideLength = { 3.5f, 3.5f });
	bool MakeObject(StageObject* stgObject, Button* button, ModelFBX* model, const Tag& objectTag, const XMFLOAT2& sideLength = { 3.5f, 3.5f });

	/// <summary>
	/// datファイルに保存
	/// </summary>
	void SaveDat();

private:
	StageObject* stgObjectEdit[OBJECT_MAX];

	bool menuON = true;
	bool buttonClick = false;
};