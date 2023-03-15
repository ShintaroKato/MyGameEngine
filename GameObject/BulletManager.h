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
	/// 弾を発射
	/// </summary>
	/// <param name="position">開始地点</param>
	/// <param name="rotation">飛ぶ方向</param>
	/// <param name="collisionAttribute">開始地点が持つ当たり判定の属性</param>
	/// <param name="velocityMagnification">速度の倍率(初期値{ 0, 0, 1})</param>
	/// <param name="power">攻撃力</param>
	/// <param name="through">オブジェクトを突き抜けるか否か</param>
	void Fire(XMFLOAT3 position, XMFLOAT3 rotation, unsigned short collisionAttribute, float velocityMagnification = 1.0f, float power = 1.0f, bool through = false);

	/// <summary>
	/// 弾を発射
	/// </summary>
	/// <param name="position">開始地点</param>
	/// <param name="rotation">飛ぶ方向</param>
	/// <param name="collisionAttribute">開始地点が持つ当たり判定の属性</param>
	/// <param name="ex_col">判定から除外するコライダー</param>
	/// <param name="velocityMagnification">速度の倍率(初期値{ 0, 0, 1})</param>
	/// <param name="power">攻撃力</param>
	/// <param name="power">オブジェクトを突き抜けるか否か</param>
	void Fire(XMFLOAT3 position, XMFLOAT3 rotation, unsigned short collisionAttribute, BaseCollider* ex_col, float velocityMagnification = 1.0f, float power = 1.0f, bool through = false);

	/// <summary>
	/// 弾を全て削除
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