#include "SceneStageEdit.h"
#include "SceneManager.h"
#include "PlaneCursor.h"

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

	for (int i = 0; i < OBJECT_MAX; i++)
	{
		if(stgObjects.size() <= i)
		{
			if (i == 0)
			{
				stgObjectEdit[i] = StageObject::Create(modelCastle);
				stgObjectEdit[i]->SetTag(CASTLE_OBJECT);
				stgObjectEdit[i]->SetRadius(5.0f);
				stgObjectEdit[i]->SetUsedState(USED);
				stgObjectEdit[i]->SetNumber(i);
			}
			else
			{
				stgObjectEdit[i] = StageObject::Create(modelCubeRed);
				stgObjectEdit[i]->SetNumber(i);
			}
		}
		else stgObjectEdit[i] = LoadStage(i);
	}


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

	//for (int i = 0; i < OBJECT_MAX; i++)
	//{
	//	SceneBase::LoadStage(stgObject[i]);
	//}

	PlaneCursor::Initialize(objCursor);

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
		for (int i = 0; i < OBJECT_MAX; i++)
		{
			SceneBase::SaveStage(stgObjectEdit[i]);
		}

		SceneManager::SetScene(TITLE);

		return;
	}

	camera->SetTarget(player->GetPosition());
	camera->SetEye({ 0,30,-100 });
	if (input->PushMouse(MOUSE_RIGHT))
	{
		player->SetCameraMoveFlag(true);
	}
	else
	{
		player->SetCameraMoveFlag(false);
	}

	SceneBase::Update();

	menuON = true;
	buttonClick = false;

	MenuUpdate();

	for (int i = 0; i < OBJECT_MAX; i++)
	{
		if (stgObjectEdit[i]->GetUsedState() != UNUSED && !buttonClick)
		{
			stgObjectEdit[i]->Update();
		}

		if (stgObjectEdit[i]->GetDragFlag()) menuON = false;
	}

	PlaneCursor::Update();
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

	for (int i = 0; i < OBJECT_MAX; i++)
	{
		if (stgObjectEdit[i]->GetUsedState() != UNUSED)
		{
			stgObjectEdit[i]->Draw();
		}
	}

	objWall->Draw();

	if(!menuON) PlaneCursor::Draw();

	ObjectOBJ::PostDraw();

	// FBXモデル
	//player->ObjectFBX::Draw(dxCommon->GetCmdList());

#pragma endregion

#pragma region 前景スプライト

	// スプライト描画前処理
	spriteCommon->PreDraw(dxCommon->GetCmdList());


	// スプライト描画
	MenuDraw();

	// テキスト描画
	text->DrawAll();

	spriteCommon->PostDraw();


#pragma endregion

#pragma endregion グラフィックスコマンド
}

void SceneStageEdit::MenuUpdate()
{
	if (!menuON) return;


	for (int i = 0; i < OBJECT_MAX; i++)
	{
		if (!menuON) break;

		if (MakeObject(stgObjectEdit[i], buttonRed, modelCubeRed, RED_OBJECT, { 1.0f, 5.0f })) break;
		if (MakeObject(stgObjectEdit[i], buttonGreen, modelCubeGreen, GREEN_OBJECT)) break;
		if (MakeObject(stgObjectEdit[i], buttonBlue, modelCubeBlue, BLUE_OBJECT)) break;
	}

	buttonTitle->Update();
	buttonRed->Update();
	buttonGreen->Update();
	buttonBlue->Update();
	spriteCursor->Update();
}

void SceneStageEdit::MenuDraw()
{
	if (!menuON) return;

	buttonTitle->Draw();
	buttonRed->Draw();
	buttonGreen->Draw();
	buttonBlue->Draw();
	spriteCursor->Draw();
}

bool SceneStageEdit::MakeObject(StageObject* stgObject, Button* button, ModelOBJ* model, const Tag& objectTag, const XMFLOAT2& sideLength)
{
	if (button->Click(MOUSE_LEFT) && stgObject->GetUsedState() == UNUSED)
	{
		stgObject->SetModel(model);
		stgObject->SetTag(objectTag);
		stgObject->SetSquareSideLength(sideLength.x, sideLength.y);
		stgObject->SetUsedState(WAITING);
		stgObject->Update();
		button->SetClickFlag(false);

		SetCursorPos(WinApp::window_width / 2, WinApp::window_height / 2);
		PlaneCursor::Update();

		menuON = false;
		buttonClick = true;
		return true;
	}

	return false;
}

bool SceneStageEdit::MakeObject(StageObject* stgObject, Button* button, ModelFBX* model, const Tag& objectTag, const XMFLOAT2& sideLength)
{
	if (button->Click(MOUSE_LEFT) && stgObject->GetUsedState() == UNUSED)
	{
		stgObject->SetModel(model);
		stgObject->SetTag(objectTag);
		stgObject->SetSquareSideLength(sideLength.x, sideLength.y);
		stgObject->SetUsedState(WAITING);
		stgObject->Update();
		button->SetClickFlag(false);

		SetCursorPos(WinApp::window_width / 2, WinApp::window_height / 2);
		PlaneCursor::Update();

		menuON = false;
		buttonClick = true;
		return true;
	}

	return false;
}
