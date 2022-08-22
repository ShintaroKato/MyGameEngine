#include "PostEffectScene.h"

PostEffectScene::PostEffectScene()
{
}

PostEffectScene::~PostEffectScene()
{
	delete fbxAnimTest;
}

void PostEffectScene::Initialize(DirectXCommon* dxCommon, SpriteCommon* spriteCommon, Input* input, Audio* audio)
{
	SceneBase::Initialize(dxCommon, spriteCommon, input, audio);
	// カメラ生成
	camera = new Camera();
	camera->Initialize(WinApp::window_width, WinApp::window_height);

	//ObjectOBJ::SetCamera(camera);
	ObjectOBJ::SetDevice(dxCommon->GetDev());

	ObjectFBX::SetCamera(camera);
	ObjectFBX::SetDevice(dxCommon->GetDev());

	// スプライト共通テクスチャ読み込み
	spriteCommon->LoadTexture(0, "debugfont.png");
	spriteCommon->LoadTexture(100, "loading.png");

	// テキスト
	text = Text::GetInstance();
	text->Initialize(spriteCommon, 0);

	// スプライト
	spriteBG = Sprite::Create(spriteCommon, 100, { 0,0 }, { 0,0 });
	spriteBG->Update();

	// obj.からモデルデータ読み込み
	modelSphere = ModelOBJ::LoadObj("sphere", true);
	// 3Dオブジェクト生成
	objSphere = ObjectOBJ::Create();
	// オブジェクトにモデルを紐づける
	objSphere->SetModelOBJ(modelSphere);

	objSphere->SetPosition({ 0,0,0 });
	objSphere->SetCamera(camera);
	objSphere->Update();

	fbxModelAnim = FBXLoader::GetInstance()->LoadModelFromFile("boneTest");
	fbxAnimTest = ObjectFBX::Create();
	fbxAnimTest->SetModelFBX(fbxModelAnim);
	fbxAnimTest->SetPosition({ 0,-10,-80 });
	fbxAnimTest->SetRotation({ 0,0,0 });
	fbxAnimTest->SetAnimationNumber(0);
	fbxAnimTest->Update();

	camera->SetTarget({ 0,0,0 });
	camera->SetEye({ 0,0,-90 });
	camera->SetTarget(fbxAnimTest->GetPosition());
	camera->Update();
}

void PostEffectScene::Update()
{
	camera->SetTarget(objSphere->GetPosition());

	camera->Update();

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


	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) ||
		input->PushKey(DIK_A) || input->PushKey(DIK_D))
	{
		XMFLOAT3 rot = objSphere->GetRotation();

		if (input->PushKey(DIK_W))
		{
			rot.x++;
		}
		else if (input->PushKey(DIK_S))
		{
			rot.x--;
		}
		else if (input->PushKey(DIK_D))
		{
			rot.y++;
		}
		else if (input->PushKey(DIK_A))
		{
			rot.y--;
		}

		objSphere->SetRotation(rot);
	}

	fbxAnimTest->Update();
	objSphere->Update();

	spriteBG->Update();
}

void PostEffectScene::Draw()
{
#pragma region 背景スプライト

	// スプライト描画前処理
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// スプライト描画
	//spriteBG->Draw();

	spriteCommon->PostDraw();

#pragma endregion

#pragma region 3Dオブジェクト

	// 3Dオブジェクト描画前処理
	ObjectOBJ::PreDraw(dxCommon->GetCmdList());

	objSphere->Draw();

	ObjectOBJ::PostDraw();

	//fbxAnimTest->Draw(dxCommon->GetCmdList());

#pragma endregion

#pragma region 前景スプライト

	// スプライト描画前処理
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// スプライト描画
	spriteBG->Draw();

	spriteCommon->PostDraw();

	// テキスト描画
	//text->DrawAll(dxCommon->GetCmdList());

#pragma endregion

#pragma endregion グラフィックスコマンド
}
