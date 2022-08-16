#include "SceneBase.h"

XMFLOAT3 SceneBase::tmp[OBJECT_MAX];

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
	spriteCommon->LoadTexture(0, "debugfont.png");
	spriteCommon->LoadTexture(1, "background.png");

	// テキスト
	text = Text::GetInstance();
	text->Initialize(spriteCommon, 0);

	// スプライト
	spriteBG = Sprite::Create(spriteCommon, 1, { 0,0 }, { 0,0 });
	spriteBG->Update();

	// .objからモデルデータ読み込み
	modelSkydome = ModelOBJ::LoadObj("skydome");
	modelGround = ModelOBJ::LoadObj("ground");
	modelPlayer = ModelOBJ::LoadObj("player");
	modelEnemy = ModelOBJ::LoadObj("player");
	modelCubeRed = ModelOBJ::LoadObj("cube64Red");
	modelCubeGreen = ModelOBJ::LoadObj("cube64Green");
	modelCubeBlue = ModelOBJ::LoadObj("cube64Blue");
	modelCastle = ModelOBJ::LoadObj("small_castle");

	// 3Dオブジェクト生成
	objSkydome = ObjectOBJ::Create();
	// オブジェクトにモデルを紐づける
	objSkydome->SetModelOBJ(modelSkydome);

	objGround = TouchableObject::Create(modelGround);

	objSkydome->SetScale({ 5,5,5 });
	objGround->ObjectOBJ::SetScale({ 5,5,5 });

	for (int i = 0; i < CUBE_RED_MAX; i++)
	{
		objCubeRed[i] = GameObject::Create(modelCubeRed);
		objCubeGreen[i] = GameObject::Create(modelCubeGreen);
		objCubeBlue[i] = GameObject::Create(modelCubeBlue);
	}
	objCastle = GameObject::Create(modelCastle);

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		enemy[i] = Enemy::Create(modelEnemy);
		enemy[i]->SetInGame(false);
	}

	player = Player::Create(modelPlayer);
	player->ObjectOBJ::SetCamera(camera);
}

void SceneBase::Update()
{
	// カメラ
	camera->Update();

	// ゲーム用オブジェクト
	player->Update();
	for (int i = 0; i < 10; i++)
	{
		objCubeRed[i]->Update();
		objCubeGreen[i]->Update();
		objCubeBlue[i]->Update();
	}
	objCastle->Update();

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		enemy[i]->Update();
	}

	// obj更新
	objSkydome->Update();
	objGround->Update();

	// fbx更新

	// スプライト
	spriteBG->Update();
}
