#include "SceneStageEdit.h"

SceneStageEdit::SceneStageEdit()
{
}

SceneStageEdit::~SceneStageEdit()
{
	delete player;
}

void SceneStageEdit::Initialize(DirectXCommon* dxCommon, SpriteCommon* sprCommon, Input* input, Audio* audio)
{
	SceneBase::Initialize(dxCommon, sprCommon, input, audio);

	buttonRed->SetPosition({ 20,20 });
	buttonRed->SetSize({ 64,64 });
	buttonRed->Update();

	buttonGreen->SetPosition({ 20,20 + 64});
	buttonGreen->SetSize({ 64,64});
	buttonGreen->Update();

	buttonBlue->SetPosition({ 20,20 + 64 * 2 });
	buttonBlue->SetSize({ 64,64 });
	buttonBlue->Update();

	for (int i = 0; i < CUBE_RED_MAX; i++)
	{
		objCubeRed[i]->ObjectOBJ::SetPosition(tmp[i]);
		objCubeGreen[i]->ObjectOBJ::SetPosition(tmp[i + 10]);
		objCubeBlue[i]->ObjectOBJ::SetPosition(tmp[i + 20]);
	}
	objCastle->ObjectOBJ::SetPosition(tmp[99]);

	objSkydome->SetScale({ 5,5,5 });
	objGround->ObjectOBJ::SetScale({ 5,5,5 });

	player->SetAllive(false);
	player->SetCameraDistance(80);
	player->SetInGameFlag(false);

	camera->SetTarget(player->GetPosition());
	camera->SetEye({ 0,50,-100 });
}

void SceneStageEdit::Update()
{
	if (input->TriggerKey(DIK_ESCAPE))
	{
		SceneManager::SetScene(TITLE);

		for (int i = 0; i < CUBE_RED_MAX; i++)
		{
			tmp[i] = objCubeRed[i]->ObjectOBJ::GetPosition();
			tmp[i + 10] = objCubeGreen[i]->ObjectOBJ::GetPosition();
			tmp[i + 20] = objCubeBlue[i]->ObjectOBJ::GetPosition();
		}
		tmp[99] = objCastle->ObjectOBJ::GetPosition();
	}

	camera->SetTarget(player->GetPosition());

	for (int i = 0; i < 10; i++)
	{
		if (buttonRed->Click(MOUSE_LEFT) && !objCubeRed[i]->GetUsedFlag())
		{
			objCubeRed[i]->SetUsedFlag(true);
			buttonRed->SetClickFlag(false);
			break;
		}
		if (buttonGreen->Click(MOUSE_LEFT) && !objCubeGreen[i]->GetUsedFlag())
		{
			objCubeGreen[i]->SetUsedFlag(true);
			buttonGreen->SetClickFlag(false);
			break;
		}
		if (buttonBlue->Click(MOUSE_LEFT) && !objCubeBlue[i]->GetUsedFlag())
		{
			objCubeBlue[i]->SetUsedFlag(true);
			buttonBlue->SetClickFlag(false);
			break;
		}
	}

	buttonRed->Update();
	buttonGreen->Update();
	buttonBlue->Update();

	SceneBase::Update();
}

void SceneStageEdit::Draw()
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

	// OBJモデル
	ObjectOBJ::PreDraw(dxCommon->GetCmdList());

	objSkydome->Draw();
	objGround->ObjectOBJ::Draw();

	for (int i = 0; i < 10; i++)
	{
		if (objCubeRed[i]->GetUsedFlag()) objCubeRed[i]->ObjectOBJ::Draw();
		if (objCubeGreen[i]->GetUsedFlag()) objCubeGreen[i]->ObjectOBJ::Draw();
		if (objCubeBlue[i]->GetUsedFlag()) objCubeBlue[i]->ObjectOBJ::Draw();
	}

	objCastle->ObjectOBJ::Draw();

	objWall->Draw();

	ObjectOBJ::PostDraw();

	// FBXモデル
	//player->ObjectFBX::Draw(dxCommon->GetCmdList());

#pragma endregion

#pragma region 前景スプライト

	// スプライト描画前処理
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	buttonRed->Draw();
	buttonGreen->Draw();
	buttonBlue->Draw();
	// スプライト描画
	//spriteBG->Draw();
	// テキスト描画
	text->DrawAll();

	spriteCommon->PostDraw();


#pragma endregion

#pragma endregion グラフィックスコマンド
}
