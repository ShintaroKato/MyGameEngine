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

	for (int i = 0; i < stgObjects.size(); i++)
	{
		stgObjects[i]->ResetStatus();
		stgObjects[i]->SetInGameFlag(false);
		stgObjectEdit.push_front(*stgObjects[i]);
	}
	// �󂯓n�����I�������z���S����
	stgObjects.clear();

	buttonTitle->SetPosition({ 0,0 });
	buttonTitle->SetSize({ 128,64 });

	buttonRed->SetPosition({ 20,20 + 64 });
	buttonRed->SetSize({ 64,64 });
	buttonRed->Update();

	buttonBlue->SetPosition({ 20,20 + 64 * 2.5f });
	buttonBlue->SetSize({ 64,64 });
	buttonBlue->Update();

	spriteUIFrame->SetPosition({ 64,20 + 32 });
	spriteUIFrame->SetSize({ 128,64 * 3.5f });
	spriteUIFrame->SetAnchorPoint({ 0.5f,0.0f });
	spriteUIFrame->Update();

	spriteGuide1->SetPosition({ WinApp::window_width + 64, WinApp::window_height });
	spriteGuide1->SetAnchorPoint({ 1.0f, 1.0f });
	spriteGuide1->Update();
	spriteGuide2->SetPosition({ WinApp::window_width + 64, WinApp::window_height });
	spriteGuide2->SetAnchorPoint({ 1.0f, 1.0f });
	spriteGuide2->Update();

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
	MenuUpdate();

	for (std::forward_list<StageObject>::iterator it = stgObjectEdit.begin(); it != stgObjectEdit.end(); it++)
	{
		if (!buttonClick)
		{
			it->Update();
		}

		if (it->GetDragFlag()) menuActivate = false;
	}
	// �J��������̋����ŕ��בւ�
	SortObjectCameraDistance();
	// ���g�p��Ԃ̃I�u�W�F�N�g���폜
	stgObjectEdit.remove_if([](StageObject& x) { return x.GetUsedState() == UNUSED; });

	PlaneCursor::Update();
}

void SceneStageEdit::Draw()
{
#pragma region �w�i�X�v���C�g

	// �X�v���C�g�`��O����
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// �X�v���C�g�`��
	//spriteBG->Draw();

	spriteCommon->PostDraw();

#pragma endregion

#pragma region 3D�I�u�W�F�N�g

	// 3D�I�u�W�F�N�g�`��O����

	// OBJ���f��
	ObjectOBJ::PreDraw(dxCommon->GetCmdList());

	objSkydome->Draw();
	objSkydomeSpace->Draw();
	objGroundGrid->ObjectOBJ::Draw();

	objWall->Draw();

	if(!menuActivate) PlaneCursor::Draw();

	for (std::forward_list<StageObject>::iterator it = stgObjectEdit.begin(); it != stgObjectEdit.end(); it++)
	{
		it->Draw();
	}

	ObjectOBJ::PostDraw();

	// FBX���f��
	//player->ObjectFBX::Draw(dxCommon->GetCmdList());

#pragma endregion

#pragma region �O�i�X�v���C�g

	// �X�v���C�g�`��O����
	spriteCommon->PreDraw(dxCommon->GetCmdList());


	// �X�v���C�g�`��
	MenuDraw();

	// �e�L�X�g�`��
	text->DrawAll();

	spriteCommon->PostDraw();


#pragma endregion

#pragma endregion �O���t�B�b�N�X�R�}���h
}

void SceneStageEdit::MenuUpdate()
{
	if (!menuActivate) return;

	MakeObject(buttonRed, modelCubeRed, RED_OBJECT, { 1.0f, 5.0f });
	MakeObject(buttonBlue, modelCubeBlue, OFFENCE_OBJECT);

	buttonTitle->Update();
	buttonRed->Update();
	buttonBlue->Update();
	spriteCursor->Update();
}

void SceneStageEdit::MenuDraw()
{
	if (!menuActivate)
	{
		spriteGuide2->Draw();
		return;
	}

	spriteUIFrame->Draw();
	buttonTitle->Draw();
	buttonRed->Draw();
	buttonBlue->Draw();
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
	// �~���\�[�g
	stgObjectEdit.sort();
}