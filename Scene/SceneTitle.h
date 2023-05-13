#pragma once
#include "SceneBase.h"

/// <summary>
/// ゲームシーン
/// </summary>
class SceneTitle : public SceneBase
{
public: // メンバ関数

	/// <summary>
	/// コンストクラタ
	/// </summary>
	SceneTitle();

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
	/// 補間でカメラを動かす(シーン切り替え時の演出)
	/// </summary>
	void CameraAnimation();

private:
	// カメラ制御用
	XMFLOAT3 startPos{};
	float nowCount = 0;
	float maxCount = 30;
	size_t startIndex = 1;
};