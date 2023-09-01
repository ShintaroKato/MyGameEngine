#include "SceneBase.h"
#include <algorithm>
#include "SafeDelete.h"

std::vector<StageObject*> SceneBase::stgObjects;

SceneBase::~SceneBase()
{
	safe_delete(camera);

	safe_delete(spriteTitle);
	safe_delete(spriteCursor);
	safe_delete(spriteWave);
	safe_delete(spriteWaveClear);
	safe_delete(spriteWaveFailed);
	safe_delete(spriteWaveFinal);
	safe_delete(spriteFractionBar);
	safe_delete(spritePause);
	safe_delete(spriteGuide1);
	safe_delete(spriteGuide2);
	safe_delete(spriteGuideMenu);
	safe_delete(spriteGuideTitle);
	safe_delete(spriteUIWindowBlue);
	safe_delete(spriteUIWindowYellow);
	safe_delete(spriteObjectGuideWall);
	safe_delete(spriteObjectGuideTower);

	safe_delete(buttonBarrier);
	safe_delete(buttonBuilding);
	safe_delete(buttonTower01);
	safe_delete(buttonBlack);
	safe_delete(buttonTitle);
	safe_delete(buttonStart);
	safe_delete(buttonEdit);
	safe_delete(buttonRetry);
	safe_delete(buttonNext);
	safe_delete(buttonObjects);

	safe_delete(meterPlayerHP);
	safe_delete(meterCastleHP);

	safe_delete(modelSkydome);
	safe_delete(modelSkydomeSpace);
	safe_delete(modelGroundGrid);
	safe_delete(modelGroundGridLine);
	safe_delete(modelPlayer);

	safe_delete(objSkydome);
	safe_delete(objSkydomeSpace);
	safe_delete(objGroundGrid);

	safe_delete(modelFbxPlayer);

	safe_delete(objCursor);

	safe_delete(player);

	for (int i = 0; i < 256; i++)
	{
		if (i < 2)					safe_delete(objGroundGridLine[i]);
		if (i < NumberSpriteCount)	safe_delete(numbers[i]);
		if (i < ENEMY_TYPE_COUNT)	safe_delete(modelEnemy[i]);
		if (i < 8)					safe_delete(objWall[i]);
		if (i < WEAPON_MAX)			safe_delete(weapon[i]);
	}
}

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
	spriteCommon->LoadTexture(button_barrier, "barrier01.png");
	spriteCommon->LoadTexture(button_tower01, "tower01.png");
	spriteCommon->LoadTexture(button_building, "building_02.png");
	spriteCommon->LoadTexture(button_wall, "wall02.png");
	spriteCommon->LoadTexture(button_title, "button_title.png");
	spriteCommon->LoadTexture(button_back, "button_back.png");
	spriteCommon->LoadTexture(button_start, "button_start.png");
	spriteCommon->LoadTexture(button_edit, "button_edit.png");
	spriteCommon->LoadTexture(button_retry, "button_retry.png");
	spriteCommon->LoadTexture(button_next, "button_next.png");
	spriteCommon->LoadTexture(button_objects, "button_objects.png");
	spriteCommon->LoadTexture(cursor, "cursor.png");
	spriteCommon->LoadTexture(player_HP, "player_HP.png");
	spriteCommon->LoadTexture(castle_HP, "castle_HP.png");
	spriteCommon->LoadTexture(wave, "wave.png");
	spriteCommon->LoadTexture(wave_clear, "wave_clear.png");
	spriteCommon->LoadTexture(wave_failed, "wave_failed.png");
	spriteCommon->LoadTexture(wave_final, "wave_final.png");
	spriteCommon->LoadTexture(number, "number.png");
	spriteCommon->LoadTexture(fraction_bar, "fraction_bar.png");
	spriteCommon->LoadTexture(pause, "pause.png");
	spriteCommon->LoadTexture(guide01, "guide01.png");
	spriteCommon->LoadTexture(guide02, "guide02.png");
	spriteCommon->LoadTexture(guide03, "guide03.png");
	spriteCommon->LoadTexture(guide_menu, "guide_menu.png");
	spriteCommon->LoadTexture(guide_title, "guide_title.png");
	spriteCommon->LoadTexture(ui_frame, "UI_frame.png");
	spriteCommon->LoadTexture(name_frame, "name_plate.png");
	spriteCommon->LoadTexture(guide_wall, "guide_text_wall.png");
	spriteCommon->LoadTexture(guide_tower, "guide_text_tower.png");

	// テキスト
	text = Text::GetInstance();
	text->Initialize(spriteCommon, debug_font);

	// スプライト
	spriteTitle = Sprite::Create(spriteCommon, title, { 0,0 }, { 0,0 });
	buttonBarrier = Button::Create(spriteCommon, button_barrier, { 0,0 }, { 0,0 });
	buttonBuilding = Button::Create(spriteCommon, button_building, { 0,0 }, { 0,0 });
	buttonTower01 = Button::Create(spriteCommon, button_tower01, { 0,0 }, { 0,0 });
	buttonWall = Button::Create(spriteCommon, button_wall, { 0,0 }, { 0,0 });
	buttonTitle = Button::Create(spriteCommon, button_title, { 0,0 }, { 0,0 });
	buttonBack = Button::Create(spriteCommon, button_back, { 0,0 }, { 0,0 });
	buttonStart = Button::Create(spriteCommon, button_start, { 0,0 }, { 0,0 });
	buttonEdit = Button::Create(spriteCommon, button_edit, { 0,0 }, { 0,0 });
	buttonRetry = Button::Create(spriteCommon, button_retry, { 0,0 }, { 0,0 });
	buttonNext = Button::Create(spriteCommon, button_next, { 0,0 }, { 0,0 });
	buttonObjects = Button::Create(spriteCommon, button_objects, { 0,0 }, { 0,0 });
	spriteCursor = Sprite::Create(spriteCommon, cursor, { 0,0 }, { 0,0 });
	spriteCursor->SetSize({ 16,16 });

	for(int i = 0;i<NumberSpriteCount;i++)
	{
		numbers[i] = Number::Create(spriteCommon, number, 1, { 0,0 }, { 0,0 });
	}
	numbers[enemy_count]->SetDigit(3);
	numbers[enemy_count]->SetAnchorPoint({0.5f, 0.5f});
	numbers[enemy_count_max]->SetDigit(3);
	numbers[enemy_count_max]->SetAnchorPoint({0.5f, 0.5f});

	meterPlayerHP = Meter::Create(spriteCommon, square_red, square_blue, player_HP, { 0,0 });
	meterCastleHP = Meter::Create(spriteCommon, square_red, square_blue, castle_HP, { 0,0 });
	spriteFractionBar = Sprite::Create(spriteCommon, fraction_bar, { 0,0 }, { 0,0 });
	spriteWave = Sprite::Create(spriteCommon, wave, { 0,0 }, { 0.5f,0.5f });
	spriteWaveClear = Sprite::Create(spriteCommon, wave_clear, { 0,0 }, { 0.5f,0.5f });
	spriteWaveFailed = Sprite::Create(spriteCommon, wave_failed, { 0,0 }, { 0.5f,0.5f });
	spriteWaveFinal = Sprite::Create(spriteCommon, wave_final, { 0,0 }, { 0.5f,0.5f });
	spritePause = Sprite::Create(spriteCommon, pause, { 0,0 }, { 0.5f,0.5f });
	spriteGuide1 = Sprite::Create(spriteCommon, guide01, { 0,0 });
	spriteGuide2 = Sprite::Create(spriteCommon, guide02, { 0,0 });
	spriteGuide3 = Sprite::Create(spriteCommon, guide03, { 0,0 });
	spriteGuideMenu = Sprite::Create(spriteCommon, guide_menu, { 0,0 });
	spriteGuideTitle = Sprite::Create(spriteCommon, guide_title, { 0,0 });
	spriteUIWindowBlue = Sprite::Create(spriteCommon, ui_frame, { 0,0 });
	spriteUIWindowYellow = Sprite::Create(spriteCommon, name_frame, { 0,0 });
	spriteObjectGuideWall = Sprite::Create(spriteCommon, guide_wall, { 0,0 });
	spriteObjectGuideTower = Sprite::Create(spriteCommon, guide_tower, { 0,0 });

	// .objからモデルデータ読み込み
	modelSkydome = ModelOBJ::LoadObj("skydome", true);
	modelSkydomeSpace = ModelOBJ::LoadObj("skydome_space", true);
	modelGroundGrid = ModelOBJ::LoadObj("512MeshPlane");
	modelGroundGridLine = ModelOBJ::LoadObj("512MeshPlaneGrid");

	modelPlayer = ModelOBJ::LoadObj("robo_white");
	modelEnemy[STRAIGHT] = ModelOBJ::LoadObj("robo_black");
	modelEnemy[FLYING] = ModelOBJ::LoadObj("space_ship_01");
	modelEnemy[ROUTE_SEARCH] = ModelOBJ::LoadObj("robo_black");
	modelBarrier = ModelOBJ::LoadObj("barrier01");
	modelBuilding = ModelOBJ::LoadObj("building_02");
	modelTower = ModelOBJ::LoadObj("tower01");
	modelCastle = ModelOBJ::LoadObj("Castle");
	modelWall = ModelOBJ::LoadObj("wall");
	modelStageWall = ModelOBJ::LoadObj("wall02");
	modelWeapon = ModelOBJ::LoadObj("sword2");
	modelCursor = ModelOBJ::LoadObj("cursor_plane");

	// .fbxからモデルデータ読み込み

	// 3Dオブジェクト生成
	objSkydome = ObjectOBJ::Create();
	// オブジェクトにモデルを紐づける
	objSkydome->SetModelOBJ(modelSkydome);
	objSkydome->SetScale({ 8,8,8 });
	objSkydome->SetShadingMode(1);

	objSkydomeSpace = ObjectOBJ::Create();
	objSkydomeSpace->SetModelOBJ(modelSkydomeSpace);
	objSkydomeSpace->SetScale({ 7.0f,7.0f,7.0f });
	objSkydomeSpace->SetShadingMode(1);

	// 外壁を設置
	fieldSize = 150.0f;
	XMFLOAT3 scale = { 3.0f,1,2.0f };
	for (int i = 0; i < 8; i++)
	{
		objWall[i] = ObjectOBJ::Create();
		objWall[i]->SetModelOBJ(modelStageWall);
		objWall[i]->SetScale(scale);

		float radius = 30.0f * scale.x * 0.6f;
		float posX = fieldSize;
		float posZ = fieldSize;

		float rot = 0;
		if (i < 4)
		{
			rot += 90;
			posZ -= radius;
		}
		else
		{
			posX -= radius;
		}

		int num = i % 4;

		if (num == 2 || num == 3)
		{
			posX = -posX;
		}
		if (num == 1 || num == 3)
		{
			posZ = -posZ;
		}

		objWall[i]->SetRotation({ 0,rot,0 });
		objWall[i]->SetPosition({ posX,0,posZ });
	}
	fieldSize = 128.0f;

	objGroundGrid = TouchableObject::Create(modelGroundGrid);
	objGroundGrid->Update();
	MeshCollider* col = static_cast<MeshCollider*>(objGroundGrid->ObjectOBJ::GetCollider());
	tri = col->GetTriangle();

	for (int i = 0; i < 2; i++)
	{
		objGroundGridLine[i] = ObjectOBJ::Create();
		objGroundGridLine[i]->SetModelOBJ(modelGroundGridLine);
		objGroundGridLine[i]->SetScale({10,10,10});
		objGroundGridLine[i]->SetPosition({ 0,0.002f,0 });

		if (i == 1)
		{
			objGroundGridLine[i]->SetRotation({ 0,90,0 });
			objGroundGridLine[i]->SetPosition({ 0,0.005f,0 });
		}
	}

	objCursor = ObjectOBJ::Create();
	objCursor->SetModelOBJ(modelCursor);
	objCursor->SetShadingMode(1);
	objCursor->SetScale({ 5.0f,5.0f,5.0f });

	weapon[0] = Weapon::Create(modelWeapon);
	//weapon[0]->SetScale({ 5,5,5 });

	player = Player::Create(modelPlayer);
	player->SetCamera(camera);
	player->SetWeapon(weapon[0]);

	particle = ParticleManager::GetInstance();
	particle->SetCamera(camera);

	if (stgObjects.size() <= 0)
	{
		stgObjects.push_back(StageObject::Create(modelCastle));
		stgObjects[0]->SetTag(STAGE_OBJECT_CASTLE);
		stgObjects[0]->SetRadius(9.0f);
		stgObjects[0]->SetUsedState(USED);
	}

	camera->SetInGameFlag(false);
	camera->SetCameraControlFlag(true);
	camera->SetRotation();
}

void SceneBase::Update()
{
	// ゲーム用オブジェクト

	for (int i = 0; i < tri.size(); i++)
	{
		ParticleEmitter::EmitY_AxisDir(1, 16,
			{
				tri[i].p0.m128_f32[0],
				tri[i].p0.m128_f32[1],
				tri[i].p0.m128_f32[2]
			}, 0.25f, { 0.3f,0.3f,1.0f,1 }, { 0,0,1,1 });
	}

	objSkydomeSpace->UpdateUV({ -0.00002f, -0.00002f });

	for (int i = 0; i < 2; i++)
	{
		objGroundGridLine[i]->UpdateUV({ 0.0f,0.001f });
	}

	objSkydome->Update();
	objSkydomeSpace->Update();
	objGroundGrid->Update();
	for (int i = 0; i < 2; i++)
	{
		objGroundGridLine[i]->Update();
	}
	for (int i = 0; i < 8; i++)
	{
		objWall[i]->Update();
	}
	player->SpawnAnimation();
	player->Update();
	weapon[0]->Update();

	// スプライト
	spriteTitle->Update();
	// パーティクル
	particle->Update();
	// カメラ
	camera->SetTarget({
		player->GetPosition().x,
		player->GetPosition().y + 2,
		player->GetPosition().z });

	camera->Update();
}

void SceneBase::LoadStage(StageObject* stageObjectReceive, StageObject* stageObjectSend)
{
	stageObjectSend->ResetStatus();
	stageObjectSend->SetInGameFlag(false);
	stageObjectReceive = stageObjectSend;
}

void SceneBase::SortObjectCameraDistance()
{
	// ラムダ式で降順ソート
	std::sort(stgObjects.begin(), stgObjects.end(),
		[](StageObject* a, StageObject* b)
		{
			return a->GetCameraDistance() > b->GetCameraDistance();
		});
}
