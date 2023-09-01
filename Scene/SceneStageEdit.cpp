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
	buttonBack->SetPosition({ 0,0 });
	buttonBack->SetSize({ 128,64 });

	buttonObjects->SetPosition({ 64,96 });
	buttonObjects->SetSize({ 128,64 });
	buttonObjects->SetAnchorPoint({ 0.5f,0.5f });

	spriteUIWindowBlue->SetPosition(buttonObjects->GetPosition());
	spriteUIWindowBlue->SetSize({ 128,0 });
	spriteUIWindowBlue->SetAnchorPoint({ 0.5f,0.0f });
	spriteUIWindowBlue->Update();

	XMFLOAT2 uiPos = spriteUIWindowBlue->GetPosition();
	float pos_offset = 90;

	buttonBarrier->SetPosition({ uiPos.x, uiPos.y + pos_offset });
	buttonBarrier->SetSize({ 64,64 });
	buttonBarrier->SetAnchorPoint({ 0.5f,0.5f });
	buttonBarrier->Update();

	buttonTower01->SetPosition({ uiPos.x, uiPos.y + pos_offset * 2.0f });
	buttonTower01->SetSize({ 64,64 });
	buttonTower01->SetAnchorPoint({ 0.5f,0.5f });
	buttonTower01->Update();

	buttonBuilding->SetPosition({ uiPos.x, uiPos.y + pos_offset * 3.0f });
	buttonBuilding->SetSize({ 64,64 });
	buttonBuilding->SetAnchorPoint({ 0.5f,0.5f });
	buttonBuilding->Update();

	buttonWall->SetPosition({ uiPos.x, uiPos.y + pos_offset * 4.0f });
	buttonWall->SetSize({ 64,64 });
	buttonWall->SetAnchorPoint({ 0.5f,0.5f });
	buttonWall->SetColor({ 0.0f,0.0f,0.0f,1.0f });
	buttonWall->Update();

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

	player->SetAlive(false);
	player->SetInGameFlag(false);

	camera->SetTarget(player->GetPosition());
	camera->SetEye({ 60,0,0 }, 150);
}

void SceneStageEdit::Update()
{
	spriteCursor->SetPosition(input->GetMousePos2());

	if (input->TriggerKey(DIK_ESCAPE) || buttonBack->Click(MOUSE_LEFT))
	{
		// シーン遷移可能であればtrue
		bool result = true;

		for (std::forward_list<StageObject>::iterator it = stgObjectEdit.begin(); it != stgObjectEdit.end(); it++)
		{
			if (it->GetTag() == STAGE_OBJECT_DEFAULT) continue;

			if (it->GetUsedState() == WAITING ||
				it->GetDragFlag() == true)
			{
				// 設置待機中のオブジェクトがあったらfalse
				result = false;
				break;
			}

			stgObjects.push_back(it->GetInstance());
		}

		if(result)
		{
			// タイトル画面に戻る
			SceneManager::SetScene(TITLE);
			return;
		}
		else
		{
			// 一度登録していたオブジェクトを全て消去
			stgObjects.clear();
		}
	}

	if (input->PushMouse(MOUSE_RIGHT))  camera->SetCameraMoveFlag(true);
	else camera->SetCameraMoveFlag(false);


	menuActive = true;
	buttonClick = false;
	UpdateMenu();

	for (std::forward_list<StageObject>::iterator it = stgObjectEdit.begin(); it != stgObjectEdit.end(); it++)
	{
		if (!buttonClick)
		{
			it->Update();
		}

		if (it->GetDragFlag()) menuActive = false;
	}
	// カメラからの距離で並べ替え
	SortObjectCameraDistance();
	// 未使用状態のオブジェクトを削除
	stgObjectEdit.remove_if([](StageObject& x) { return x.GetUsedState() == UNUSED; });

	PlaneCursor::Update();
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
	objSkydomeSpace->Draw();
	objGroundGrid->ObjectOBJ::Draw();

	for (int i = 0; i < 2; i++)
	{
		objGroundGridLine[i]->Draw();
	}

	for (int i = 0; i < 8; i++)
	{
		objWall[i]->Draw();
	}

	PlaneCursor::Draw();

	for (std::forward_list<StageObject>::iterator it = stgObjectEdit.begin(); it != stgObjectEdit.end(); it++)
	{
		it->Draw();
	}

	ParticleManager::GetInstance()->Draw(dxCommon->GetCmdList());

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
	if (!menuActive) return;

	if (buttonObjects->Click(MOUSE_LEFT))
	{
		if (!windowActive) windowActive = true;
		else			   windowActive = false;
	}

	// 8f でUIのウィンドウの縦幅を 256px にする
	int count = 8;
	float max = 512;
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
	buttonBack->Update();
	buttonObjects->Update();
	spriteCursor->Update();

	if (!windowActive) return;

	MakeObject(buttonBarrier, modelBarrier, STAGE_OBJECT_BARRIER, { 1.0f, 5.0f });
	MakeObject(buttonTower01, modelTower, STAGE_OBJECT_OFFENCE);
	MakeObject(buttonBuilding, modelBuilding, STAGE_OBJECT_BUILDING);
	MakeObject(buttonWall, modelStageWall, STAGE_OBJECT_WALL);

	if (buttonBarrier->Point() || buttonTower01->Point() || buttonBuilding->Point() || buttonWall->Point())
	{
		spriteUIWindowYellow->Update();

		if (buttonBarrier->Point())
		{
			spriteObjectGuideWall->SetPosition({
					spriteUIWindowYellow->GetPosition().x + 16,
					spriteUIWindowYellow->GetPosition().y
				});
			spriteObjectGuideWall->Update();
		}
		if (buttonTower01->Point())
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

	buttonBarrier->Update();
	buttonTower01->Update();
	buttonBuilding->Update();
	buttonWall->Update();
}

void SceneStageEdit::DrawMenu()
{
	if (!menuActive)
	{
		spriteGuide2->Draw();
		return;
	}

	spriteUIWindowBlue->Draw();

	if (listActive)
	{
		buttonBarrier->Draw();
		buttonTower01->Draw();
		buttonBuilding->Draw();
		buttonWall->Draw();

		if (buttonBarrier->Point() || buttonTower01->Point())
		{
			spriteUIWindowYellow->Draw();

			if (buttonBarrier->Point()) spriteObjectGuideWall->Draw();
			if (buttonBuilding->Point());
			if (buttonTower01->Point()) spriteObjectGuideTower->Draw();
			if (buttonWall->Point());
		}
	}

	buttonBack->Draw();
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
		//button->SetClickFlag(false);

		SetCursorPos(WinApp::window_width / 2, WinApp::window_height / 2);
		PlaneCursor::Update();

		menuActive = false;
		buttonClick = true;
		return true;
	}

	return false;
}

bool SceneStageEdit::MakeObject(Button* button, ModelFBX* model, const Tag& objectTag, const XMFLOAT2& sideLength)
{
	if (button->Hold(MOUSE_LEFT))
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

		menuActive = false;
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