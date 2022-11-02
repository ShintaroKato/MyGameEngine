#pragma once
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Audio.h"
#include "Input.h"
#include "Sprite.h"
#include "Text.h"
#include "ObjectOBJ.h"
#include "ObjectFBX.h"
#include "UI/Button.h"
#include "UI/Number.h"
#include "GameObject.h"
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
	button_r,
	button_g,
	button_b,
	button_title,
	button_start,
	button_edit,
	cursor,
	number,
};

// オブジェクトのデータを保存するための構造体
struct ObjectTmpData
{
	XMFLOAT3 pos;
	bool isUsed;
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

	static const int CUBE_RED_MAX = 10;
	static const int CUBE_GREEN_MAX = 10;
	static const int CUBE_BLUE_MAX = 10;
	static const int ENEMY_MAX = 20;
	static const int WEAPON_MAX = 1;

public:
	static ObjectTmpData tmp[OBJECT_MAX];

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
	///	ゲームマネージャーをセット
	/// </summary>
	/// <param name="gameManager">ゲームマネージャー</param>
	void SetGameManager(GameManager* gameManager) { gManager = gameManager; }

	/// <summary>
	/// 作成したステージを保存
	/// </summary>
	static void SaveStage(GameObject* gameObject);

	/// <summary>
	/// 作成したステージを読み込み
	/// </summary>
	static void LoadStage(GameObject* gameObject);

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

	Button* buttonRed = nullptr;
	Button* buttonGreen = nullptr;
	Button* buttonBlue = nullptr;
	Button* buttonTitle = nullptr;
	Button* buttonStart = nullptr;
	Button* buttonEdit = nullptr;

	Number* numberTimer = nullptr;

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

	ObjectOBJ* objWall = nullptr;
	ObjectOBJ* objSkydome = nullptr;
	TouchableObject* objGroundGrid = nullptr;

	static GameObject* objCubeRed[CUBE_RED_MAX];
	static GameObject* objCubeGreen[CUBE_GREEN_MAX];
	static GameObject* objCubeBlue[CUBE_BLUE_MAX];

	static GameObject* objCastle;

	GameManager* gManager = nullptr;

	Player* player = nullptr;
	Enemy* enemy[ENEMY_MAX]{};
	Weapon* weapon[WEAPON_MAX]{};
};