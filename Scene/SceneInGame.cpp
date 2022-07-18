#include "SceneInGame.h"

SceneInGame::SceneInGame()
{
}

SceneInGame::~SceneInGame()
{
	delete fbxAnimTest;
}

void SceneInGame::Initialize(DirectXCommon* dxCommon, SpriteCommon* sprCommon, Input* input, Audio* audio)
{
	SceneBase::Initialize(dxCommon, sprCommon, input, audio);

	// スプライト共通テクスチャ読み込み
	spriteCommon->LoadTexture(0, "debugfont.png");
	spriteCommon->LoadTexture(1, "background.png");

	// テキスト
	text = Text::GetInstance();
	text->Initialize(spriteCommon, 0);

	// スプライト
	spriteBG = Sprite::Create(spriteCommon, 1, { 0,0 }, {0,0});
	spriteBG->Update();

	// obj.からモデルデータ読み込み
	modelSphere = ModelOBJ::LoadObj("sphere", true);
	// 3Dオブジェクト生成
	objSphere = ObjectOBJ::Create();
	// オブジェクトにモデルを紐づける
	objSphere->SetModelOBJ(modelSphere);

	objSphere->SetPosition({ 0,0,10});
	objSphere->SetCamera(camera);
	objSphere->Update();

	fbxModelAnim = FBXLoader::GetInstance()->LoadModelFromFile("boneTest");
	fbxAnimTest = ObjectFBX::Create();
	fbxAnimTest->SetModel(fbxModelAnim);
	fbxAnimTest->SetPosition({ 0,-10,-80 });
	fbxAnimTest->SetRotation({ 0,0,0 });
	fbxAnimTest->SetAnimationNumber(0);
	fbxAnimTest->Update();

	camera->SetTarget({ 0,0,0 });
	camera->SetEye({ 0,0,-100 });
	camera->SetTarget(fbxAnimTest->GetPosition());
	camera->Update();
}

void SceneInGame::Update()
{
	if (input->PushKey(DIK_UP))
	{
		camera->CameraMoveVector({ 0,1,0 });
	}
	else if (input->PushKey(DIK_DOWN))
	{
		camera->CameraMoveVector({ 0,-1,0 });
	}
	else if (input->PushKey(DIK_RIGHT))
	{
		camera->CameraMoveVector({ 1,0,0 });
	}
	else if (input->PushKey(DIK_LEFT))
	{
		camera->CameraMoveVector({ -1,0,0 });
	}
	else
	{
		camera->CameraMoveVector({ 0,0,0 });
	}

	camera->SetTarget(fbxAnimTest->GetPosition());

	camera->Update();

	if (input->TriggerKey(DIK_SPACE))
	{
		SceneManager::SetScene(TITLE);
	}
	if (input->PushKey(DIK_P))
	{
		fbxAnimTest->AnimationPlay();
	}
	if (input->PushKey(DIK_S))
	{
		fbxAnimTest->AnimationStop();
	}
	if (input->PushKey(DIK_R))
	{
		fbxAnimTest->AnimationReset();
	}

	fbxAnimTest->Update();
	objSphere->Update();

	spriteBG->Update();
}

void SceneInGame::Draw()
{
#pragma region 背景スプライト

	// スプライト描画前処理
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// スプライト描画
	spriteBG->Draw();

	spriteCommon->PostDraw();

#pragma endregion

#pragma region 3Dオブジェクト

	// 3Dオブジェクト描画前処理
	ObjectOBJ::PreDraw(dxCommon->GetCmdList());

	//objSphere->Draw();

	ObjectOBJ::PostDraw();

	fbxAnimTest->Draw(dxCommon->GetCmdList());

#pragma endregion

#pragma region 前景スプライト

	// スプライト描画前処理
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// スプライト描画
	//spriteBG->Draw();

	spriteCommon->PostDraw();

	// テキスト描画
	//text->DrawAll(dxCommon->GetCmdList());

#pragma endregion

#pragma endregion グラフィックスコマンド
}
