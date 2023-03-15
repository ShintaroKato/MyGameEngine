#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>

#include "Bullet.h"

using namespace std;

class Bullet;

class BulletManager
{
public:
	static BulletManager* GetInstance();

	void Update();
	
	/// <summary>
	/// �e�𔭎�
	/// </summary>
	/// <param name="position">�J�n�n�_</param>
	/// <param name="rotation">��ԕ���</param>
	/// <param name="collisionAttribute">�J�n�n�_���������蔻��̑���</param>
	/// <param name="velocityMagnification">���x�̔{��(�����l{ 0, 0, 1})</param>
	/// <param name="power">�U����</param>
	/// <param name="through">�I�u�W�F�N�g��˂������邩�ۂ�</param>
	void Fire(XMFLOAT3 position, XMFLOAT3 rotation, unsigned short collisionAttribute, float velocityMagnification = 1.0f, float power = 1.0f, bool through = false);

	/// <summary>
	/// �e�𔭎�
	/// </summary>
	/// <param name="position">�J�n�n�_</param>
	/// <param name="rotation">��ԕ���</param>
	/// <param name="collisionAttribute">�J�n�n�_���������蔻��̑���</param>
	/// <param name="ex_col">���肩�珜�O����R���C�_�[</param>
	/// <param name="velocityMagnification">���x�̔{��(�����l{ 0, 0, 1})</param>
	/// <param name="power">�U����</param>
	/// <param name="power">�I�u�W�F�N�g��˂������邩�ۂ�</param>
	void Fire(XMFLOAT3 position, XMFLOAT3 rotation, unsigned short collisionAttribute, BaseCollider* ex_col, float velocityMagnification = 1.0f, float power = 1.0f, bool through = false);

	/// <summary>
	/// �e��S�č폜
	/// </summary>
	void Delete();

private:
	std::forward_list<Bullet> bullets;

private:
	BulletManager() = default;
	BulletManager(const BulletManager&) = delete;
	~BulletManager() = default;
	BulletManager& operator=(const BulletManager&) = delete;
};