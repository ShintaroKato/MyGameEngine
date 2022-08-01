#include "SceneBase.h"

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

	// obj.からモデルデータ読み込み
	modelGround = ModelOBJ::LoadObj("ground", true);
	modelSkydome = ModelOBJ::LoadObj("skydome", true);
	modelSphere = ModelOBJ::LoadObj("sphere", true);

	// 3Dオブジェクト生成
	objGround = ObjectOBJ::Create();
	objSkydome = ObjectOBJ::Create();
	objSphere = ObjectOBJ::Create();
	// オブジェクトにモデルを紐づける
	objGround->SetModelOBJ(modelGround);
	objSkydome->SetModelOBJ(modelSkydome);
	objSphere->SetModelOBJ(modelSphere);
}

void SceneBase::Update()
{
	camera->Update();

	objGround->Update();
	objSkydome->Update();
	objSphere->Update();

	spriteBG->Update();
}
