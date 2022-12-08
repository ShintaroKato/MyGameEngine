#pragma once
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Audio.h"
#include "Input.h"
#include "Sprite.h"
#include "Text.h"
#include "ObjectOBJ.h"
#include "ObjectFBX.h"
#include "Button.h"
#include "Number.h"
#include "Meter.h"
#include "StageObject.h"
#include "Player.h"
#include "Enemy.h"
#include "Weapon.h"
#include "TouchableObject.h"
#include "GameManager.h"
#define OBJECT_MAX 100

enum SpriteNo
{
	debug_font,
	title,
	square_red,
	square_green,
	square_blue,
	button_title,
	button_start,
	button_edit,
	cursor,
	number,
	player_HP,
	castle_HP,
	wave_clear,
	wave_failed,
	pause,
};

// オブジェクトのデータを保存するための構造体
struct ObjectTmpData
{
	XMFLOAT3 pos;
	UsedState used;
	std::string tag;

	bool isSaved = false;
};

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

	static const int ENEMY_MAX = 20;
	static const int WEAPON_MAX = 1;

public:
	static std::vector<StageObject*> stgObjects;

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

	/// <summary>
	/// 作成したステージを保存
	/// </summary>
	static void SaveStage(StageObject* stageObject);

	/// <summary>
	/// 作成したステージを読み込み
	/// </summary>
	static StageObject* LoadStage(int i = 0);

protected:

	DirectXCommon* dxCommon = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;

	Camera* camera = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	Text* text = nullptr;

	Sprite* spriteTitle = nullptr;
	Sprite* spriteCursor = nullptr;
	Sprite* spriteWaveClear = nullptr;
	Sprite* spriteWaveFailed = nullptr;
	Sprite* spritePause = nullptr;

	Button* buttonRed = nullptr;
	Button* buttonGreen = nullptr;
	Button* buttonBlue = nullptr;
	Button* buttonTitle = nullptr;
	Button* buttonStart = nullptr;
	Button* buttonEdit = nullptr;

	Number* numberTimer = nullptr;
	Number* numberWaitTimer = nullptr;

	Meter* meterPlayerHP = nullptr;
	Meter* meterCastleHP = nullptr;

	ModelOBJ* modelSkydome = nullptr;
	ModelOBJ* modelGround = nullptr;
	ModelOBJ* modelGroundGrid = nullptr;
	ModelOBJ* modelPlayer = nullptr;
	ModelOBJ* modelEnemy = nullptr;
	ModelOBJ* modelCubeRed = nullptr;
	ModelOBJ* modelCubeGreen = nullptr;
	ModelOBJ* modelCubeBlue = nullptr;
	ModelOBJ* modelCastle = nullptr;
	ModelOBJ* modelWall = nullptr;
	ModelOBJ* modelWeapon = nullptr;
	ModelOBJ* modelCursor = nullptr;

	ObjectOBJ* objWall = nullptr;
	ObjectOBJ* objSkydome = nullptr;
	TouchableObject* objGroundGrid = nullptr;

	StageObject* objCastle;

	ObjectOBJ* objCursor = nullptr;

	Player* player = nullptr;
	Enemy* enemy[ENEMY_MAX]{};
	Weapon* weapon[WEAPON_MAX]{};
};