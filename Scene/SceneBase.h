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
#include "ParticleManager.h"
#include "ParticleEmitter.h"
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
	button_retry,
	button_next,
	cursor,
	number,
	player_HP,
	castle_HP,
	wave,
	wave_clear,
	wave_failed,
	wave_final,
	wave_number,
	pause,
	guide01,
	guide02,
	ui_frame,
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

	static const int ENEMY_TYPE_COUNT = EnemyType::TYPE_COUNT;
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
	/// 作成したステージを読み込み
	/// </summary>
	/// <param name="stageObjectReceive">読み込まれたオブジェクトを受け取る</param>
	/// <param name="stageObjectSend">送られるオブジェクト</param>
	/// <returns></returns>
	static void LoadStage(StageObject* stageObjectReceive, StageObject* stageObjectSend);

	/// <summary>
	/// ステージオブジェクトをカメラからの距離で並べ替える
	/// (遠くにあるほど配列の上に入る)
	/// </summary>
	static void SortObjectCameraDistance();

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
	Sprite* spriteWave = nullptr;
	Sprite* spriteWaveClear = nullptr;
	Sprite* spriteWaveFailed = nullptr;
	Sprite* spriteWaveFinal = nullptr;
	Sprite* spritePause = nullptr;
	Sprite* spriteGuide1 = nullptr;
	Sprite* spriteGuide2 = nullptr;
	Sprite* spriteUIFrame = nullptr;

	Button* buttonRed = nullptr;
	Button* buttonGreen = nullptr;
	Button* buttonBlue = nullptr;
	Button* buttonTitle = nullptr;
	Button* buttonStart = nullptr;
	Button* buttonEdit = nullptr;
	Button* buttonRetry = nullptr;
	Button* buttonNext = nullptr;

	Number* numberTimer = nullptr;
	Number* numberWaitTimer = nullptr;
	Number* numberWave = nullptr;

	Meter* meterPlayerHP = nullptr;
	Meter* meterCastleHP = nullptr;

	ParticleManager* particle = nullptr;

	ModelOBJ* modelSkydome = nullptr;
	ModelOBJ* modelSkydomeSpace = nullptr;
	ModelOBJ* modelGround = nullptr;
	ModelOBJ* modelGroundGrid = nullptr;
	ModelOBJ* modelPlayer = nullptr;
	ModelOBJ* modelEnemy[ENEMY_TYPE_COUNT]{};
	ModelOBJ* modelCubeRed = nullptr;
	ModelOBJ* modelCubeGreen = nullptr;
	ModelOBJ* modelCubeBlue = nullptr;
	ModelOBJ* modelCastle = nullptr;
	ModelOBJ* modelWall = nullptr;
	ModelOBJ* modelWeapon = nullptr;
	ModelOBJ* modelCursor = nullptr;

	ObjectOBJ* objWall = nullptr;
	ObjectOBJ* objSkydome = nullptr;
	ObjectOBJ* objSkydomeSpace = nullptr;
	TouchableObject* objGroundGrid = nullptr;

	StageObject* objCastle;

	ObjectOBJ* objCursor = nullptr;

	Player* player = nullptr;
	Weapon* weapon[WEAPON_MAX]{};

	// 星空の天球の回転量
	XMFLOAT3 skydomeRot{};
};