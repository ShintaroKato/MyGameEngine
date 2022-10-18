#include "SceneBase.h"

XMFLOAT3 SceneBase::tmp[OBJECT_MAX];
bool SceneBase::tmpFlag[OBJECT_MAX];

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
	spriteCommon->LoadTexture(button_r, "256x256Red.png");
	spriteCommon->LoadTexture(button_g, "256x256Green.png");
	spriteCommon->LoadTexture(button_b, "256x256Blue.png");
	spriteCommon->LoadTexture(button_title, "button_title.png");
	spriteCommon->LoadTexture(button_start, "button_start.png");
	spriteCommon->LoadTexture(button_edit, "button_edit.png");

	// テキスト
	text = Text::GetInstance();
	text->Initialize(spriteCommon, debug_font);

	// スプライト
	spriteTitle = Sprite::Create(spriteCommon, title, { 0,0 }, { 0,0 });
	buttonRed = Button::Create(spriteCommon, button_r, { 0,0 }, { 0,0 });
	buttonGreen = Button::Create(spriteCommon, button_g, { 0,0 }, { 0,0 });
	buttonBlue = Button::Create(spriteCommon, button_b, { 0,0 }, { 0,0 });
	buttonTitle = Button::Create(spriteCommon, button_title, { 0,0 }, { 0,0 });
	buttonStart = Button::Create(spriteCommon, button_start, { 0,0 }, { 0,0 });
	buttonEdit = Button::Create(spriteCommon, button_edit, { 0,0 }, { 0,0 });

	spriteTitle->Update();

	// .objからモデルデータ読み込み
	modelSkydome = ModelOBJ::LoadObj("skydome");
	modelGround = ModelOBJ::LoadObj("ground");
	modelGroundGrid = ModelOBJ::LoadObj("512MeshPlane");
	modelPlayer = ModelOBJ::LoadObj("chr_sword");
	modelEnemy = ModelOBJ::LoadObj("player");
	modelCubeRed = ModelOBJ::LoadObj("cube64Red");
	modelCubeGreen = ModelOBJ::LoadObj("brokenBlock");
	modelCubeBlue = ModelOBJ::LoadObj("cube64Blue");
	modelCastle = ModelOBJ::LoadObj("small_castle");
	modelWall = ModelOBJ::LoadObj("square_wall");
	modelWeapon = ModelOBJ::LoadObj("sword2");

	// 3Dオブジェクト生成
	objSkydome = ObjectOBJ::Create();
	// オブジェクトにモデルを紐づける
	objSkydome->SetModelOBJ(modelSkydome);
	objSkydome->SetScale({ 5,5,5 });

	objWall = ObjectOBJ::Create();
	objWall->SetModelOBJ(modelWall);
	objWall->SetScale({ 12,1,12 });

	objGroundGrid = TouchableObject::Create(modelGroundGrid);
	objGroundGrid->ObjectOBJ::SetScale({ 10,10,10 });

	for (int i = 0; i < CUBE_RED_MAX; i++)
	{
		objCubeRed[i] = GameObject::Create(modelCubeRed);
		objCubeGreen[i] = GameObject::Create(modelCubeGreen);
		objCubeBlue[i] = GameObject::Create(modelCubeBlue);
	}
	objCastle = GameObject::Create(modelCastle);
	objCastle->SetRadius(8.0f);

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
	for (int i = 0; i < 10; i++)
	{
		if (objCubeRed[i]->GetUsedFlag()) objCubeRed[i]->Update();
		if (objCubeGreen[i]->GetUsedFlag()) objCubeGreen[i]->Update();
		if (objCubeBlue[i]->GetUsedFlag()) objCubeBlue[i]->Update();
	}
	objCastle->Update();

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		enemy[i]->Update();
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

void SceneBase::SaveStage()
{

}

void SceneBase::LoadStage()
{
}
