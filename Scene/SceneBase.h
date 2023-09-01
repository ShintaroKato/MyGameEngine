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
#include "Math/Interpolate.h"

#define OBJECT_MAX 100

enum SpriteNo
{
	debug_font,
	title,
	square_red,
	square_green,
	square_blue,
	button_barrier,
	button_tower01,
	button_building,
	button_wall,
	button_title,
	button_back,
	button_start,
	button_edit,
	button_retry,
	button_next,
	button_objects,
	cursor,
	number,
	fraction_bar, // �����̐�
	player_HP,
	castle_HP,
	wave,
	wave_clear,
	wave_failed,
	wave_final,
	pause,
	guide01,
	guide02,
	guide03,
	guide_menu,
	guide_title,
	guide_wall,
	guide_tower,
	ui_frame,
	name_frame,
};

enum NumberSprite
{
	enemy_count,
	enemy_count_max,
	wait_timer,
	wave_number,

	NumberSpriteCount
};

// �I�u�W�F�N�g�̃f�[�^��ۑ����邽�߂̍\����
struct ObjectTmpData
{
	XMFLOAT3 pos;
	UsedState used;
	std::string tag;

	bool isSaved = false;
};

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

	static const int ENEMY_TYPE_COUNT = EnemyType::TYPE_COUNT;
	static const int WEAPON_MAX = 1;

public:
	static std::vector<StageObject*> stgObjects;

public:

	~SceneBase();

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

	/// <summary>
	/// �쐬�����X�e�[�W��ǂݍ���
	/// </summary>
	/// <param name="stageObjectReceive">�ǂݍ��܂ꂽ�I�u�W�F�N�g���󂯎��</param>
	/// <param name="stageObjectSend">������I�u�W�F�N�g</param>
	/// <returns></returns>
	static void LoadStage(StageObject* stageObjectReceive, StageObject* stageObjectSend);

	/// <summary>
	/// �X�e�[�W�I�u�W�F�N�g���J��������̋����ŕ��בւ���
	/// (�����ɂ���قǔz��̏�ɓ���)
	/// </summary>
	void SortObjectCameraDistance();

protected:

	DirectXCommon* dxCommon = nullptr;
	SpriteCommon* spriteCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;

	Camera* camera = nullptr;

	/// <summary>
	/// �Q�[���V�[���p
	/// </summary>
	Text* text = nullptr;

	Sprite* spriteTitle = nullptr;
	Sprite* spriteCursor = nullptr;
	Sprite* spriteWave = nullptr;
	Sprite* spriteWaveClear = nullptr;
	Sprite* spriteWaveFailed = nullptr;
	Sprite* spriteWaveFinal = nullptr;
	Sprite* spriteFractionBar = nullptr; // �����̐�
	Sprite* spritePause = nullptr;
	Sprite* spriteGuide1 = nullptr;
	Sprite* spriteGuide2 = nullptr;
	Sprite* spriteGuide3 = nullptr;
	Sprite* spriteGuideMenu = nullptr;
	Sprite* spriteGuideTitle = nullptr;
	Sprite* spriteUIWindowBlue = nullptr;
	Sprite* spriteUIWindowYellow = nullptr;
	Sprite* spriteObjectGuideWall = nullptr;
	Sprite* spriteObjectGuideTower = nullptr;

	Button* buttonBarrier = nullptr;
	Button* buttonBuilding = nullptr;
	Button* buttonTower01 = nullptr;
	Button* buttonWall = nullptr;
	Button* buttonBlack = nullptr;
	Button* buttonTitle = nullptr;
	Button* buttonBack = nullptr;
	Button* buttonStart = nullptr;
	Button* buttonEdit = nullptr;
	Button* buttonRetry = nullptr;
	Button* buttonNext = nullptr;
	Button* buttonObjects = nullptr;

	Number* numbers[NumberSpriteCount];

	Meter* meterPlayerHP = nullptr;
	Meter* meterCastleHP = nullptr;

	ParticleManager* particle = nullptr;

	ModelOBJ* modelSkydome = nullptr;
	ModelOBJ* modelSkydomeSpace = nullptr;
	ModelOBJ* modelGroundGrid = nullptr;
	ModelOBJ* modelGroundGridLine = nullptr;
	ModelOBJ* modelPlayer = nullptr;
	ModelOBJ* modelEnemy[ENEMY_TYPE_COUNT]{};
	ModelOBJ* modelBarrier = nullptr;
	ModelOBJ* modelBuilding = nullptr;
	ModelOBJ* modelTower = nullptr;
	ModelOBJ* modelCastle = nullptr;
	ModelOBJ* modelWall = nullptr;
	ModelOBJ* modelStageWall = nullptr;
	ModelOBJ* modelWeapon = nullptr;
	ModelOBJ* modelCursor = nullptr;

	ObjectOBJ* objWall[8]{};
	ObjectOBJ* objSkydome = nullptr;
	ObjectOBJ* objSkydomeSpace = nullptr;
	ObjectOBJ* objGroundGridLine[2]{};
	TouchableObject* objGroundGrid = nullptr;

	ModelFBX* modelFbxPlayer = nullptr;

	StageObject* objCastle;

	ObjectOBJ* objCursor = nullptr;

	Player* player = nullptr;
	Weapon* weapon[WEAPON_MAX]{};

	// �t�B�[���h�̍L��
	float fieldSize = 0;

	std::vector<Triangle> tri{};
};