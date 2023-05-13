#include "SceneTitle.h"
#include "SceneManager.h"

XMFLOAT3 operator+(XMFLOAT3 l, XMFLOAT3 r)
{
	XMFLOAT3 result{};
	result.x = l.x + r.x;
	result.y = l.y + r.y;
	result.z = l.z + r.z;

	return result;
}

XMFLOAT3 operator/(XMFLOAT3 l, float r)
{
	XMFLOAT3 result{};
	result.x = l.x / r;
	result.y = l.y / r;
	result.z = l.z / r;

	return result;
}

XMFLOAT3 operator*(XMFLOAT3 l, float r)
{
	XMFLOAT3 result{};
	result.x = l.x * r;
	result.y = l.y * r;
	result.z = l.z * r;

	return result;
}

SceneTitle::SceneTitle()
{
}

void SceneTitle::Initialize(DirectXCommon* dxCommon, SpriteCommon* sprCommon, Input* input, Audio* audio)
{
	SceneBase::Initialize(dxCommon, sprCommon, input, audio);

	for (int i = 0; i < stgObjects.size(); i++)
	{
		stgObjects[i]->SetInGameFlag(true);
		stgObjects[i]->ResetStatus();
		if (stgObjects[i]->GetTag() == STAGE_OBJECT_CASTLE) objCastle = stgObjects[i];
	}

	player->SetAllive(false);
	player->SetInGameFlag(false);

	camera->SetCameraDistance(80);
	camera->SetTarget(player->GetPosition());
	camera->SetEye({ 0,30,-100 });

	buttonStart->SetPosition({ WinApp::window_width / 2,WinApp::window_height - 256 });
	buttonStart->SetSize({ 128,64 });
	buttonStart->SetAnchorPoint({ 0.5f,0.5f });

	buttonEdit->SetPosition({ WinApp::window_width / 2,WinApp::window_height - 256 + 64 + 20 });
	buttonEdit->SetSize({ 128,64 });
	buttonEdit->SetAnchorPoint({ 0.5f,0.5f });

	SceneBase::Update();
}

void SceneTitle::Update()
{
	spriteCursor->SetPosition(input->GetMousePos2());

	if (input->TriggerKey(DIK_1) || buttonEdit->Click(MOUSE_LEFT))
	{
		SceneManager::SetScene(EDIT);

		return;
	}
	if (nowCount == 0 && ((input->TriggerKey(DIK_2) || buttonStart->Click(MOUSE_LEFT))))
	{
		camera->SetCameraControlFlag(false);
		startPos = camera->GetEye();
	}
	if(!camera->GetCameraControlFlag())
	{
		CameraAnimation();

		if(startIndex == 3 && nowCount >= 30)
		{
			SceneManager::SetScene(GAME);

			return;
		}
	}

	if (input->PushMouse(MOUSE_RIGHT))
	{
		camera->SetCameraMoveFlag(true);
	}
	else
	{
		camera->SetCameraMoveFlag(false);
	}

	buttonEdit->Update();

	buttonStart->Update();

	spriteCursor->Update();

	SceneBase::Update();

	for (int i = 0; i < stgObjects.size(); i++)
	{
		stgObjects[i]->Update();
	}

	SortObjectCameraDistance();
}

void SceneTitle::Draw()
{
#pragma region �w�i�X�v���C�g

	// �X�v���C�g�`��O����
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	// �X�v���C�g�`��
	spriteTitle->Draw();

	spriteCommon->PostDraw();

#pragma endregion

#pragma region 3D�I�u�W�F�N�g

	// 3D�I�u�W�F�N�g�`��O����
	// OBJ���f��
	ObjectOBJ::PreDraw(dxCommon->GetCmdList());

	objSkydome->Draw();
	objSkydomeSpace->Draw();
	objGroundGrid->ObjectOBJ::Draw();

	for (int i = 0; i < 8; i++)
	{
		objWall[i]->Draw();
	}

	for (int i = 0; i < stgObjects.size(); i++)
	{
		stgObjects[i]->ObjectOBJ::Draw();
	}

	ObjectOBJ::PostDraw();

	particle->Draw(dxCommon->GetCmdList());

	// FBX���f��
	//player->ObjectFBX::Draw(dxCommon->GetCmdList());

#pragma endregion

#pragma region �O�i�X�v���C�g

	// �X�v���C�g�`��O����
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	buttonEdit->Draw();

	buttonStart->Draw();

	// �X�v���C�g�`��
	spriteTitle->Draw();
	spriteCursor->Draw();
	// �e�L�X�g�`��
	//text->DrawAll(dxCommon->GetCmdList());

	spriteCommon->PostDraw();

#pragma endregion

#pragma endregion �O���t�B�b�N�X�R�}���h
}

void SceneTitle::CameraAnimation()
{
	float timeRate = min(nowCount / maxCount, 1.0f);
	XMFLOAT3 targetPos = objCastle->GetPosition();
	XMVECTOR vpos{};

	// �J�����̈ʒu
	XMFLOAT3 point = { 0,150,-150 };
	std::vector<XMFLOAT3> points = { // �ʉߓ_
		startPos,
		startPos,
		(startPos + point) / 4,
		point,
		targetPos + XMFLOAT3(0, 10, -5),
		targetPos,
	};

	vpos = Interpolate::Spline(points, startIndex, timeRate);
	XMFLOAT3 pos = {
		vpos.m128_f32[0],
		vpos.m128_f32[1],
		vpos.m128_f32[2]
	};

	camera->SetEye(pos);

	// �J�����̒����_
	std::vector<XMFLOAT3> targetPoints = { // �ʉߓ_
		{0,0,0},
		{0,0,0},
		targetPos / 4,
		targetPos / 2,
		targetPos,
		targetPos,
	};

	vpos = Interpolate::Spline(targetPoints, startIndex, timeRate);
	targetPos = {
		vpos.m128_f32[0],
		vpos.m128_f32[1],
		vpos.m128_f32[2]
	};

	camera->SetTarget(targetPos);
	

	if (timeRate < 1.0f)
	{
		nowCount++;
	}
	else
	{
		nowCount = 0.0f;
		startIndex++;
	}
}
