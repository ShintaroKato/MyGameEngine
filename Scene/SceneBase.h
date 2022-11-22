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
#include "UI/Meter.h"
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

// �I�u�W�F�N�g�̃f�[�^��ۑ����邽�߂̍\����
struct ObjectTmpData
{
	XMFLOAT3 pos;
	bool isUsed;
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

	static const int CUBE_RED_MAX = 10;
	static const int CUBE_GREEN_MAX = 10;
	static const int CUBE_BLUE_MAX = 10;
	static const int ENEMY_MAX = 20;
	static const int WEAPON_MAX = 1;

public:
	static ObjectTmpData tmp[OBJECT_MAX];

public:
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
	/// �쐬�����X�e�[�W��ۑ�
	/// </summary>
	static void SaveStage(GameObject* gameObject);

	/// <summary>
	/// �쐬�����X�e�[�W��ǂݍ���
	/// </summary>
	static void LoadStage(GameObject* gameObject, bool isInGame = false);

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

	ObjectOBJ* objWall = nullptr;
	ObjectOBJ* objSkydome = nullptr;
	TouchableObject* objGroundGrid = nullptr;

	static GameObject* objCubeRed[CUBE_RED_MAX];
	static GameObject* objCubeGreen[CUBE_GREEN_MAX];
	static GameObject* objCubeBlue[CUBE_BLUE_MAX];

	static GameObject* objCastle;

	Player* player = nullptr;
	Enemy* enemy[ENEMY_MAX]{};
	Weapon* weapon[WEAPON_MAX]{};
};