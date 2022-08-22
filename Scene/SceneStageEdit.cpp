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

	if (input->TriggerKey(DIK_R))
	{
		objCubeRed[0]->SetPosition({ 25,0,50 });
		objCubeGreen[0]->SetPosition({ -25,0,50 });
		objCubeBlue[0]->SetPosition({ 25,0,-25 });

		objCubeRed[1]->SetPosition({ 50,0,25 });
		objCubeGreen[1]->SetPosition({ -50,0,25 });
		objCubeBlue[1]->SetPosition({ -25,0,-25 });

		objCastle->SetPosition({ 0,0,0 });
	}

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

	objCubeRed[0]->ObjectOBJ::Draw();
	objCubeGreen[0]->ObjectOBJ::Draw();
	objCubeBlue[0]->ObjectOBJ::Draw();

	objCubeRed[1]->ObjectOBJ::Draw();
	objCubeGreen[1]->ObjectOBJ::Draw();
	objCubeBlue[1]->ObjectOBJ::Draw();

	objCastle->ObjectOBJ::Draw();

	objWall->Draw();

	ObjectOBJ::PostDraw();

	// FBXモデル
	//player->ObjectFBX::Draw(dxCommon->GetCmdList());

#pragma endregion

#pragma region 前景スプライト

	// スプライト描画前処理
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// スプライト描画
	//spriteBG->Draw();
	// テキスト描画
	text->DrawAll();

	spriteCommon->PostDraw();


#pragma endregion

#pragma endregion グラフィックスコマンド
}
