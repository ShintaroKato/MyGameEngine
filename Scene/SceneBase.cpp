#include "SceneBase.h"
#include <algorithm>

std::vector<StageObject*> SceneBase::stgObjects;

void SceneBase::Initialize(DirectXCommon* dxCommon, SpriteCommon* spriteCommon, Input* input, Audio* audio)
{
	// nullptrチェック
	assert(dxCommon);
	assert(spriteCommon);
	assert(input);

	this->dxCommon = dxCommon;
	this->spriteCommon = spriteCommon;
	this->input = input;
	this->audio = audio;

	// カメラ生成
	camera = new Camera();
	camera->Initialize(WinApp::window_width, WinApp::window_height);

	ObjectOBJ::SetCamera(camera);
	ObjectOBJ::SetDevice(this->dxCommon->GetDev());

	ObjectFBX::SetCamera(camera);
	ObjectFBX::SetDevice(this->dxCommon->GetDev());

	// スプライト共通テクスチャ読み込み
	spriteCommon->LoadTexture(debug_font, "debugfont.png");
	spriteCommon->LoadTexture(title, "title_transparent.png");
	spriteCommon->LoadTexture(square_red, "256x256Red.png");
	spriteCommon->LoadTexture(square_green, "256x256Green.png");
	spriteCommon->LoadTexture(square_blue, "256x256Blue.png");
	spriteCommon->LoadTexture(button_title, "button_title.png");
	spriteCommon->LoadTexture(button_start, "button_start.png");
	spriteCommon->LoadTexture(button_edit, "button_edit.png");
	spriteCommon->LoadTexture(cursor, "cursor.png");
	spriteCommon->LoadTexture(number, "number.png");
	spriteCommon->LoadTexture(player_HP, "player_HP.png");
	spriteCommon->LoadTexture(castle_HP, "castle_HP.png");
	spriteCommon->LoadTexture(wave_clear, "wave_clear.png");
	spriteCommon->LoadTexture(wave_failed, "wave_failed.png");
	spriteCommon->LoadTexture(pause, "pause.png");

	// テキスト
	text = Text::GetInstance();
	text->Initialize(spriteCommon, debug_font);

	// スプライト
	spriteTitle = Sprite::Create(spriteCommon, title, { 0,0 }, { 0,0 });
	buttonRed = Button::Create(spriteCommon, square_red, { 0,0 }, { 0,0 });
	buttonGreen = Button::Create(spriteCommon, square_green, { 0,0 }, { 0,0 });
	buttonBlue = Button::Create(spriteCommon, square_blue, { 0,0 }, { 0,0 });
	buttonTitle = Button::Create(spriteCommon, button_title, { 0,0 }, { 0,0 });
	buttonStart = Button::Create(spriteCommon, button_start, { 0,0 }, { 0,0 });
	buttonEdit = Button::Create(spriteCommon, button_edit, { 0,0 }, { 0,0 });
	spriteCursor = Sprite::Create(spriteCommon, cursor, { 0,0 }, { 0,0 });
	spriteCursor->SetSize({ 16,16 });
	numberTimer = Number::Create(spriteCommon, number, 3, { 0,0 }, { 0,0 });
	numberWaitTimer = Number::Create(spriteCommon, number, 1, { 0,0 }, { 0.5f,0.5f });
	meterPlayerHP = Meter::Create(spriteCommon, square_red, square_blue, player_HP, { 0,0 });
	meterCastleHP = Meter::Create(spriteCommon, square_red, square_blue, castle_HP, { 0,0 });
	spriteWaveClear = Sprite::Create(spriteCommon, wave_clear, { 0,0 }, { 0.5f,0.5f });
	spriteWaveFailed = Sprite::Create(spriteCommon, wave_failed, { 0,0 }, { 0.5f,0.5f });
	spritePause = Sprite::Create(spriteCommon, pause, { 0,0 }, { 0.5f,0.5f });

	// .objからモデルデータ読み込み
	modelSkydome = ModelOBJ::LoadObj("skydome");
	modelGround = ModelOBJ::LoadObj("ground");
	modelGroundGrid = ModelOBJ::LoadObj("512MeshPlane");
	modelPlayer = ModelOBJ::LoadObj("player");
	modelEnemy = ModelOBJ::LoadObj("chr_sword");
	modelCubeRed = ModelOBJ::LoadObj("cube64Red");
	modelCubeGreen = ModelOBJ::LoadObj("brokenBlock");
	modelCubeBlue = ModelOBJ::LoadObj("tower01");
	modelCastle = ModelOBJ::LoadObj("small_castle");
	modelWall = ModelOBJ::LoadObj("square_wall");
	modelWeapon = ModelOBJ::LoadObj("sword2");
	modelCursor = ModelOBJ::LoadObj("cursor_plane");

	// 3Dオブジェクト生成
	objSkydome = ObjectOBJ::Create();
	// オブジェクトにモデルを紐づける
	objSkydome->SetModelOBJ(modelSkydome);
	objSkydome->SetScale({ 5,5,5 });
	objSkydome->SetShadingMode(1);

	objWall = ObjectOBJ::Create();
	objWall->SetModelOBJ(modelWall);
	objWall->SetScale({ 12,1,12 });

	objGroundGrid = TouchableObject::Create(modelGroundGrid);
	objGroundGrid->ObjectOBJ::SetScale({ 10,10,10 });

	objCursor = ObjectOBJ::Create();
	objCursor->SetModelOBJ(modelCursor);
	objCursor->SetShadingMode(1);
	objCursor->SetScale({ 5.0f,5.0f,5.0f });

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		enemy[i] = Enemy::Create(modelEnemy);
		enemy[i]->SetInGame(false);
		enemy[i]->SetScale({ 2,2,2 });
	}

	weapon[0] = Weapon::Create(modelWeapon);

	player = Player::Create(modelPlayer);
	player->ObjectOBJ::SetCamera(camera);
	player->SetWeapon(weapon[0]);
}

void SceneBase::Update()
{
	// カメラ
	camera->Update();

	// ゲーム用オブジェクト
	player->Update();

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if(GameManager::GetWaitTimer() <= 0) enemy[i]->Update();
	}

	// obj更新
	objSkydome->Update();
	objGroundGrid->Update();
	objWall->Update();
	weapon[0]->Update();
	// fbx更新

	// スプライト
	spriteTitle->Update();
}

void SceneBase::SaveStage(StageObject* stageObject)
{
	if (stgObjects.size() == OBJECT_MAX - 1) return;

	if (stageObject->GetTag() != STAGE_OBJECT_DEFAULT)
	{
		for (int i = 0; i < stgObjects.size(); i++)
		{
			if (stgObjects[i]->GetNumber() == stageObject->GetNumber()) return;
		}

		stgObjects.push_back(stageObject);
	}
}

StageObject* SceneBase::LoadStage(int i)
{
	stgObjects[i]->ResetStatus();
	stgObjects[i]->SetInGameFlag(false);
	return stgObjects[i];
}
