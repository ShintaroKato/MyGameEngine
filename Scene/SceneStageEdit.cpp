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
	buttonTitle->SetPosition({ 0,0 });
	buttonTitle->SetSize({ 128,64 });

	buttonRed->SetPosition({ 20,20 + 64 });
	buttonRed->SetSize({ 64,64 });
	buttonRed->Update();

	buttonGreen->SetPosition({ 20,20 + 64 * 2});
	buttonGreen->SetSize({ 64,64 });
	buttonGreen->Update();

	buttonBlue->SetPosition({ 20,20 + 64 * 3 });
	buttonBlue->SetSize({ 64,64 });
	buttonBlue->Update();

	for (int i = 0; i < CUBE_RED_MAX; i++)
	{
		objCubeRed[i]->SetPosition(tmp[i]);
		objCubeGreen[i]->SetPosition(tmp[i + 10]);
		objCubeBlue[i]->SetPosition(tmp[i + 20]);

		objCubeRed[i]->SetUsedFlag(tmpFlag[i]);
		objCubeGreen[i]->SetUsedFlag(tmpFlag[i + 10]);
		objCubeBlue[i]->SetUsedFlag(tmpFlag[i + 20]);
	}
	objCastle->SetPosition(tmp[99]);
	objCastle->SetUsedFlag(true);

	objSkydome->SetScale({ 5,5,5 });
	objGroundGrid->ObjectOBJ::SetScale({ 5,5,5 });

	player->SetAllive(false);
	player->SetCameraDistance(80);
	player->SetInGameFlag(false);

	camera->SetTarget(player->GetPosition());
	camera->SetEye({ 0,50,-100 });
}

void SceneStageEdit::Update()
{
	spriteCursor->SetPosition(input->GetMousePos2());

	if (input->TriggerKey(DIK_ESCAPE) || buttonTitle->Click(MOUSE_LEFT))
	{
		SceneManager::SetScene(TITLE);

		for (int i = 0; i < CUBE_RED_MAX; i++)
		{
			tmp[i] = objCubeRed[i]->ObjectOBJ::GetPosition();
			tmp[i + 10] = objCubeGreen[i]->ObjectOBJ::GetPosition();
			tmp[i + 20] = objCubeBlue[i]->ObjectOBJ::GetPosition();

			tmpFlag[i] = objCubeRed[i]->GetUsedFlag();
			tmpFlag[i + 10] = objCubeGreen[i]->GetUsedFlag();
			tmpFlag[i + 20] = objCubeBlue[i]->GetUsedFlag();
		}
		tmp[99] = objCastle->ObjectOBJ::GetPosition();
		tmpFlag[99] = objCastle->GetUsedFlag();
	}

	camera->SetTarget(player->GetPosition());

	if (input->PushMouse(MOUSE_RIGHT))
	{
		player->SetCameraMoveFlag(true);
	}
	else
	{
		player->SetCameraMoveFlag(false);
	}

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

	buttonTitle->Update();
	buttonRed->Update();
	buttonGreen->Update();
	buttonBlue->Update();
	spriteCursor->Update();

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
	objGroundGrid->ObjectOBJ::Draw();

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

	buttonTitle->Draw();
	buttonRed->Draw();
	buttonGreen->Draw();
	buttonBlue->Draw();
	// スプライト描画
	spriteCursor->Draw();
	// テキスト描画
	text->DrawAll();

	spriteCommon->PostDraw();


#pragma endregion

#pragma endregion グラフィックスコマンド
}
