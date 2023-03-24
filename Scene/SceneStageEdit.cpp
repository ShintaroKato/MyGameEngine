#include "SceneStageEdit.h"
#include "SceneManager.h"
#include "PlaneCursor.h"

SceneStageEdit::SceneStageEdit()
{
}

SceneStageEdit::~SceneStageEdit()
{
}

void SceneStageEdit::Initialize(DirectXCommon* dxCommon, SpriteCommon* sprCommon, Input* input, Audio* audio)
{
	SceneBase::Initialize(dxCommon, sprCommon, input, audio);

	Initialize2D();
	Initialize3D();
}

void SceneStageEdit::Initialize2D()
{
	buttonTitle->SetPosition({ 0,0 });
	buttonTitle->SetSize({ 128,64 });

	buttonObjects->SetPosition({ 64,96 });
	buttonObjects->SetSize({ 128,64 });
	buttonObjects->SetAnchorPoint({ 0.5f,0.5f });

	spriteUIWindowBlue->SetPosition(buttonObjects->GetPosition());
	spriteUIWindowBlue->SetSize({ 128,0 });
	spriteUIWindowBlue->SetAnchorPoint({ 0.5f,0.0f });
	spriteUIWindowBlue->Update();

	XMFLOAT2 uiPos = spriteUIWindowBlue->GetPosition();
	float pos_offset = 90;

	buttonRed->SetPosition({ uiPos.x, uiPos.y + pos_offset });
	buttonRed->SetSize({ 64,64 });
	buttonRed->SetAnchorPoint({ 0.5f,0.5f });
	buttonRed->Update();

	buttonBlue->SetPosition({ uiPos.x, uiPos.y + pos_offset * 2.0f });
	buttonBlue->SetSize({ 64,64 });
	buttonBlue->SetAnchorPoint({ 0.5f,0.5f });
	buttonBlue->Update();

	spriteUIWindowYellow->SetSize({ 400, 64 * 1.5f });
	spriteUIWindowYellow->SetAnchorPoint({ 0.0f, 0.0f });
	spriteUIWindowYellow->Update();

	spriteGuide1->SetPosition({ WinApp::window_width + 64, WinApp::window_height });
	spriteGuide1->SetAnchorPoint({ 1.0f, 1.0f });
	spriteGuide1->Update();
	spriteGuide2->SetPosition({ WinApp::window_width + 64, WinApp::window_height });
	spriteGuide2->SetAnchorPoint({ 1.0f, 1.0f });
	spriteGuide2->Update();

	spriteObjectGuideWall->SetSize({
		spriteObjectGuideWall->GetSize().x / 1.2f,
		spriteObjectGuideWall->GetSize().y / 1.2f });
	spriteObjectGuideWall->SetAnchorPoint({ 0.0f,0.0f });
	spriteObjectGuideWall->Update();

	spriteObjectGuideTower->SetSize({
		spriteObjectGuideTower->GetSize().x / 1.2f,
		spriteObjectGuideTower->GetSize().y / 1.2f });
	spriteObjectGuideTower->SetAnchorPoint({ 0.0f,0.0f });
	spriteObjectGuideTower->Update();
}

void SceneStageEdit::Initialize3D()
{

	for (int i = 0; i < stgObjects.size(); i++)
	{
		stgObjects[i]->ResetStatus();
		stgObjects[i]->SetInGameFlag(false);
		stgObjectEdit.push_front(*stgObjects[i]);
	}
	// 受け渡しが終わったら配列を全消去
	stgObjects.clear();

	PlaneCursor::Initialize(objCursor);
	PlaneCursor::SetMovableRange(fieldSize);

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
		for (std::forward_list<StageObject>::iterator it = stgObjectEdit.begin(); it != stgObjectEdit.end(); it++)
		{
			if (it->GetTag() == STAGE_OBJECT_DEFAULT) continue;

			if (it->GetUsedState() == WAITING)
			{
				it->SetUsedState(USED);
			}

			stgObjects.push_back(it->GetInstance());
		}

		SceneManager::SetScene(TITLE);

		return;
	}

	camera->SetTarget(player->GetPosition());
	camera->SetEye({ 0,30,-100 });

	if (input->PushMouse(MOUSE_RIGHT))  player->SetCameraMoveFlag(true);
	else player->SetCameraMoveFlag(false);

	SceneBase::Update();

	menuActivate = true;
	buttonClick = false;
	UpdateMenu();

	for (std::forward_list<StageObject>::iterator it = stgObjectEdit.begin(); it != stgObjectEdit.end(); it++)
	{
		if (!buttonClick)
		{
			it->Update();
		}

		if (it->GetDragFlag()) menuActivate = false;
	}
	// カメラからの距離で並べ替え
	SortObjectCameraDistance();
	// 未使用状態のオブジェクトを削除
	stgObjectEdit.remove_if([](StageObject& x) { return x.GetUsedState() == UNUSED; });

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
	objSkydomeSpace->Draw();
	objGroundGrid->ObjectOBJ::Draw();

	for (int i = 0; i < 8; i++)
	{
		objWall[i]->Draw();
	}

	if(!menuActivate) PlaneCursor::Draw();

	for (std::forward_list<StageObject>::iterator it = stgObjectEdit.begin(); it != stgObjectEdit.end(); it++)
	{
		it->Draw();
	}

	ObjectOBJ::PostDraw();

	// FBXモデル
	//player->ObjectFBX::Draw(dxCommon->GetCmdList());

#pragma endregion

#pragma region 前景スプライト

	// スプライト描画前処理
	spriteCommon->PreDraw(dxCommon->GetCmdList());


	// スプライト描画
	DrawMenu();

	// テキスト描画
	text->DrawAll();

	spriteCommon->PostDraw();


#pragma endregion

#pragma endregion グラフィックスコマンド
}

void SceneStageEdit::UpdateMenu()
{
	if (!menuActivate) return;

	if (buttonObjects->Click(MOUSE_LEFT))
	{
		if (!windowActive) windowActive = true;
		else			   windowActive = false;
	}

	// 8f でUIのウィンドウの縦幅を 256px にする
	int count = 8;
	float max = 256;
	float sizeY = spriteUIWindowBlue->GetSize().y;

	if (windowActive)
	{
		if (sizeY < max) sizeY += max / count;
		else			 listActive = true;
	}
	else
	{
		if (sizeY > 0)	 sizeY -= max / count;
		listActive = false;
	}

	spriteUIWindowBlue->SetSize({ 120, sizeY });
	spriteUIWindowBlue->Update();
	buttonTitle->Update();
	buttonObjects->Update();
	spriteCursor->Update();

	if (!windowActive) return;

	MakeObject(buttonRed, modelCubeRed, RED_OBJECT, { 1.0f, 5.0f });
	MakeObject(buttonBlue, modelCubeBlue, OFFENCE_OBJECT);

	if (buttonRed->Point() || buttonBlue->Point())
	{
		spriteUIWindowYellow->Update();

		if (buttonRed->Point())
		{
			spriteObjectGuideWall->SetPosition({
					spriteUIWindowYellow->GetPosition().x + 16,
					spriteUIWindowYellow->GetPosition().y
				});
			spriteObjectGuideWall->Update();
		}
		if (buttonBlue->Point())
		{
			spriteObjectGuideTower->SetPosition({
					spriteUIWindowYellow->GetPosition().x + 16,
					spriteUIWindowYellow->GetPosition().y
				});
			spriteObjectGuideTower->Update();
		}
	}
	else
	{
		spriteUIWindowYellow->SetPosition(input->GetMousePos2());
		spriteUIWindowYellow->Update();
	}

	buttonRed->Update();
	buttonBlue->Update();
}

void SceneStageEdit::DrawMenu()
{
	if (!menuActivate)
	{
		spriteGuide2->Draw();
		return;
	}

	spriteUIWindowBlue->Draw();

	if (listActive)
	{
		buttonRed->Draw();
		buttonBlue->Draw();

		if (buttonRed->Point() || buttonBlue->Point())
		{
			spriteUIWindowYellow->Draw();

			if (buttonRed->Point()) spriteObjectGuideWall->Draw();
			if (buttonBlue->Point()) spriteObjectGuideTower->Draw();
		}
	}

	buttonTitle->Draw();
	buttonObjects->Draw();
	spriteCursor->Draw();
	spriteGuide1->Draw();
}

bool SceneStageEdit::MakeObject(Button* button, ModelOBJ* model, const Tag& objectTag, const XMFLOAT2& sideLength)
{
	if (button->Click(MOUSE_LEFT))
	{
		stgObjectEdit.push_front(*StageObject::Create(model));
		StageObject& stgObject = stgObjectEdit.front();
		stgObject.SetTag(objectTag);
		stgObject.SetSquareSideLength(sideLength.x, sideLength.y);
		stgObject.SetUsedState(WAITING);
		stgObject.Update();
		button->SetClickFlag(false);

		SetCursorPos(WinApp::window_width / 2, WinApp::window_height / 2);
		PlaneCursor::Update();

		menuActivate = false;
		buttonClick = true;
		return true;
	}

	return false;
}

bool SceneStageEdit::MakeObject(Button* button, ModelFBX* model, const Tag& objectTag, const XMFLOAT2& sideLength)
{
	if (button->Click(MOUSE_LEFT))
	{
		stgObjectEdit.push_front(*StageObject::Create(model));
		StageObject& stgObject = stgObjectEdit.front();
		stgObject.SetTag(objectTag);
		stgObject.SetSquareSideLength(sideLength.x, sideLength.y);
		stgObject.SetUsedState(WAITING);
		stgObject.Update();
		button->SetClickFlag(false);

		SetCursorPos(WinApp::window_width / 2, WinApp::window_height / 2);
		PlaneCursor::Update();

		menuActivate = false;
		buttonClick = true;
		return true;
	}

	return false;
}

void SceneStageEdit::SortObjectCameraDistance()
{
	// 降順ソート
	stgObjectEdit.sort();
}