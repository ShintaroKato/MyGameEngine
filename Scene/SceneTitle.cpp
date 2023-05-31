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

	if(!SceneManager::GetTitleEnd())
	{
		player->SetAlive(false);
		player->SetInGameFlag(false);
		player->SetPosition({ 0, 5, 0 });

		camera->SetCameraDistance(80);
		camera->SetTarget(player->GetPosition());
		camera->SetEye({ 0,30,-100 });
	}
	else
	{
		player->SetAlive(true);
		player->SetInGameFlag(true);
		player->SetPosition({
			objCastle->GetPosition().x,
			objCastle->GetPosition().y + 40,
			objCastle->GetPosition().z
			});
		player->Update();

		camera->SetCameraDistance(10);
		camera->SetTarget(player->GetPosition());
		camera->SetEye({
			player->GetPosition().x,
			player->GetPosition().y + 2,
			player->GetPosition().z - 5
			});
		camera->Update();

		cameraMoveEnd = true;
	}

	buttonStart->SetPosition({ WinApp::window_width / 2,WinApp::window_height / 2 - 50 });
	buttonStart->SetSize({ 128,64 });
	buttonStart->SetAnchorPoint({ 0.5f,0.5f });
	buttonStart->Update();

	buttonEdit->SetPosition({ WinApp::window_width / 2,WinApp::window_height / 2 - 50 + 64 + 20 });
	buttonEdit->SetSize({ 128,64 });
	buttonEdit->SetAnchorPoint({ 0.5f,0.5f });
	buttonEdit->Update();

	spriteUIWindowBlue->SetSize({ 0,256 });
	spriteUIWindowBlue->SetAnchorPoint({ 0.5f,0.5f });
	spriteUIWindowBlue->SetPosition({ WinApp::window_width / 2,WinApp::window_height / 2 });
	spriteUIWindowBlue->Update();

	spriteGuideTitle->SetAnchorPoint({ 0.5f,0.5f });
	spriteGuideTitle->SetPosition({ WinApp::window_width / 2,WinApp::window_height / 2 + 128 });
	spriteGuideTitle->Update();

	spriteGuideMenu->SetAnchorPoint({ 0.0f,0.0f });

	SceneBase::Update();
}

void SceneTitle::Update()
{
	spriteCursor->SetPosition(input->GetMousePos2());

	// �^�C�g����ʂ��I�������鑀��
	if (nowCount == 0 && !SceneManager::GetTitleEnd() &&
		(input->PushMouse(MOUSE_LEFT) || input->PushKeyAll()))
	{
		SceneManager::ChangeTitleEnd();
		startPos = camera->GetEye();
	}
	if(SceneManager::GetTitleEnd() && !cameraMoveEnd)
	{
		CameraAnimation();

		// �v���C���[�̈ʒu�����_�I�u�W�F�N�g�ɍ��킹��
		player->SetPosition({
			objCastle->GetPosition().x,
			objCastle->GetPosition().y + 40,
			objCastle->GetPosition().z
			});
		player->Update();

		if (startIndex == 3)
		{
			player->SetAlive(true);
			player->SetInGameFlag(true);
		}
		if (startIndex == 4) // �J�����̈ʒu��index4�ɗ����Ƃ�
		{
			// ���N���b�N�Ȃ��Ŏ��_�ړ��\
			camera->SetCameraControlFlag(true);
			camera->SetCameraMoveFlag(true);

			camera->SetCameraDistance(10);
			camera->SetEye({
				player->GetPosition().x,
				player->GetPosition().y + 2,
				player->GetPosition().z - 5
				});
			camera->SetTarget(player->GetPosition());

			camera->Update();

			cameraMoveEnd = true;
		}
	}


	if(!SceneManager::GetTitleEnd())
	{
		if (input->PushMouse(MOUSE_RIGHT))
		{
			camera->SetCameraMoveFlag(true);
		}
		else
		{
			camera->SetCameraMoveFlag(false);
		}
	}

	if(menuActive)
	{
		if (input->TriggerKey(DIK_2) || buttonStart->Click(MOUSE_LEFT))
		{
			SceneManager::SetScene(GAME);

			return;
		}
		if (input->TriggerKey(DIK_1) || buttonEdit->Click(MOUSE_LEFT))
		{
			SceneManager::SetScene(EDIT);

			return;
		}
	}

	if (SceneManager::GetTitleEnd())
	{
		UpdateMenu();
	}
	else
	{
		spriteCursor->SetPosition(input->GetMousePos2());
		spriteCursor->Update();
	}

	for (int i = 0; i < stgObjects.size(); i++)
	{
		stgObjects[i]->Update();
	}

	SceneBase::Update();
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

	player->ObjectOBJ::Draw();

	ObjectOBJ::PostDraw();

	particle->Draw(dxCommon->GetCmdList());

	// FBX���f��
	//player->ObjectFBX::Draw(dxCommon->GetCmdList());

#pragma endregion

#pragma region �O�i�X�v���C�g

	// �X�v���C�g�`��O����
	spriteCommon->PreDraw(dxCommon->GetCmdList());

	DrawMenu();

	// �X�v���C�g�`��
	if(!SceneManager::GetTitleEnd())
	{
		spriteTitle->Draw();
		spriteCursor->Draw();
		spriteGuideTitle->Draw();
	}
	else
	{
		spriteGuideMenu->Draw();
	}
	// �e�L�X�g�`��
	//text->DrawAll(dxCommon->GetCmdList());

	spriteCommon->PostDraw();

#pragma endregion

#pragma endregion �O���t�B�b�N�X�R�}���h
}

void SceneTitle::CameraAnimation()
{
	camera->SetCameraControlFlag(false);

	float timeRate = min(nowCount / maxCount, 1.0f);
	XMFLOAT3 cameraEndPos = {
		player->GetPosition().x,
		player->GetPosition().y + 10,
		player->GetPosition().z - 20
		};
	XMFLOAT3 targetPos = player->GetPosition();
	XMVECTOR vpos{};

	// �J�����̈ʒu
	XMFLOAT3 point = { 0,150,-150 };
	std::vector<XMFLOAT3> points = { // �ʉߓ_
		startPos,
		startPos,
		(startPos + point) / 4,
		point,
		cameraEndPos,
		cameraEndPos,
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
		targetPos,
		targetPos,
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

void SceneTitle::UpdateMenu()
{
	if (!menuActive)
	{
		// �}�E�X�J�[�\���̍��W����ʒ����ɌŒ�
		SetCursorPos(WinApp::window_width / 2, WinApp::window_height / 2);
		camera->SetCameraMoveFlag(true);

		if (input->TriggerKey(DIK_ESCAPE) || GameManager::GetFinishState() != 0)
		{
			windowActive = true;
		}
	}
	else
	{
		spriteCursor->SetPosition(input->GetMousePos2());
		spriteCursor->Update();
		camera->SetCameraMoveFlag(false);

		if (input->TriggerKey(DIK_ESCAPE) && GameManager::GetFinishState() == 0)
		{
			windowActive = false;
		}
	}

	int count = 10;
	float max = 512.0f;
	float windowSizeX = spriteUIWindowBlue->GetSize().x;

	if (windowActive)
	{
		if (windowSizeX < max)	windowSizeX += max / count;
		menuActive = true;
	}
	else
	{
		if (windowSizeX > 0)	windowSizeX -= max / count;
		else					menuActive = false;
	}

	float alpha = windowSizeX / max;
	if ((int)(alpha * 100) % 3 == 0)
	{
		alpha = 0;
	}

	buttonStart->SetColor({ 1.0f,1.0f,1.0f, alpha });
	buttonEdit->SetColor({ 1.0f,1.0f,1.0f, alpha });
	spriteUIWindowBlue->SetColor({ 1.0f,1.0f,1.0f, alpha });
	spriteUIWindowBlue->SetSize({ windowSizeX, 256.0f });

	buttonStart->Update();
	buttonEdit->Update();
	spriteUIWindowBlue->Update();
}

void SceneTitle::DrawMenu()
{
	if (menuActive)
	{
		spriteUIWindowBlue->Draw();

		if (windowActive)
		{
			buttonEdit->Draw();
			buttonStart->Draw();
		}

		spriteCursor->Draw();
	}
}