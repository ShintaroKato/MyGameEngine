#include "ParticleEmitter.h"

void ParticleEmitter::EmitAllRange(unsigned int count, unsigned int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT4 start_color, XMFLOAT4 end_color, float vel_rand_range, float accel_rand_range, float start_scale, float end_scale)
{
	for (int i = 0; i < count; i++)
	{
		// X,Y,Z全て設定した数値内でランダムに分布
		const XMFLOAT3 rnd_vel = { vel_rand_range, vel_rand_range, vel_rand_range };
		XMFLOAT3 vel = velocity;
		vel.x += (float)rand() / RAND_MAX * rnd_vel.x - rnd_vel.x / 2.0f;
		vel.y += (float)rand() / RAND_MAX * rnd_vel.y - rnd_vel.y / 2.0f;
		vel.z += (float)rand() / RAND_MAX * rnd_vel.z - rnd_vel.z / 2.0f;

		const XMFLOAT3 rnd_acc = { accel_rand_range, accel_rand_range, accel_rand_range };
		XMFLOAT3 acc{};
		acc.x = (float)rand() / RAND_MAX * rnd_acc.x - rnd_acc.x / 2.0f;
		acc.y = (float)rand() / RAND_MAX * rnd_acc.y - rnd_acc.y / 2.0f;
		acc.z = (float)rand() / RAND_MAX * rnd_acc.z - rnd_acc.z / 2.0f;

		//カラー
		XMFLOAT4 s_color = start_color;
		XMFLOAT4 e_color = end_color;

		// 追加
		ParticleManager::GetInstance()->Add(life, position, vel, acc, 1.0f, 0.0f, s_color, e_color);
	}
}

void ParticleEmitter::EmitX_AxisDir(unsigned int count, unsigned int life, XMFLOAT3 position, float velocity_X, XMFLOAT4 start_color, XMFLOAT4 end_color, float vel_rand_range, float accel_rand_range, float start_scale, float end_scale)
{
	for (int i = 0; i < count; i++)
	{
		// X,Y,Z全て設定した数値内でランダムに分布
		const XMFLOAT3 rnd_vel = { vel_rand_range, vel_rand_range, vel_rand_range };
		XMFLOAT3 vel = { velocity_X, 0, 0 };
		vel.x += (float)rand() / RAND_MAX * rnd_vel.x - rnd_vel.x / 2.0f;
		vel.y += (float)rand() / RAND_MAX * rnd_vel.y - rnd_vel.y / 2.0f;
		vel.z += (float)rand() / RAND_MAX * rnd_vel.z - rnd_vel.z / 2.0f;

		const XMFLOAT3 rnd_acc = { accel_rand_range, accel_rand_range, accel_rand_range };
		XMFLOAT3 acc{};
		acc.x = (float)rand() / RAND_MAX * rnd_acc.x - rnd_acc.x / 2.0f;
		acc.y = (float)rand() / RAND_MAX * rnd_acc.y - rnd_acc.y / 2.0f;
		acc.z = (float)rand() / RAND_MAX * rnd_acc.z - rnd_acc.z / 2.0f;

		//カラー
		XMFLOAT4 s_color = start_color;
		XMFLOAT4 e_color = end_color;

		// 追加
		ParticleManager::GetInstance()->Add(life, position, vel, acc, 1.0f, 0.0f, s_color, e_color);
	}
}

void ParticleEmitter::EmitY_AxisDir(unsigned int count, unsigned int life, XMFLOAT3 position, float velocity_Y, XMFLOAT4 start_color, XMFLOAT4 end_color, float vel_rand_range, float accel_rand_range, float start_scale, float end_scale)
{
	for (int i = 0; i < count; i++)
	{
		// X,Y,Z全て設定した数値内でランダムに分布
		const XMFLOAT3 rnd_vel = { vel_rand_range, vel_rand_range, vel_rand_range };
		XMFLOAT3 vel = { 0, velocity_Y, 0 };
		vel.x += (float)rand() / RAND_MAX * rnd_vel.x - rnd_vel.x / 2.0f;
		vel.y += (float)rand() / RAND_MAX * rnd_vel.y - rnd_vel.y / 2.0f;
		vel.z += (float)rand() / RAND_MAX * rnd_vel.z - rnd_vel.z / 2.0f;

		const XMFLOAT3 rnd_acc = { accel_rand_range, accel_rand_range, accel_rand_range };
		XMFLOAT3 acc{};
		acc.x = (float)rand() / RAND_MAX * rnd_acc.x - rnd_acc.x / 2.0f;
		acc.y = (float)rand() / RAND_MAX * rnd_acc.y - rnd_acc.y / 2.0f;
		acc.z = (float)rand() / RAND_MAX * rnd_acc.z - rnd_acc.z / 2.0f;

		//カラー
		XMFLOAT4 s_color = start_color;
		XMFLOAT4 e_color = end_color;

		// 追加
		ParticleManager::GetInstance()->Add(life, position, vel, acc, 1.0f, 0.0f, s_color, e_color);
	}
}

void ParticleEmitter::EmitZ_AxisDir(unsigned int count, unsigned int life, XMFLOAT3 position, float velocity_Z, XMFLOAT4 start_color, XMFLOAT4 end_color, float vel_rand_range, float accel_rand_range, float start_scale, float end_scale)
{
	for (int i = 0; i < count; i++)
	{
		// X,Y,Z全て設定した数値内でランダムに分布
		const XMFLOAT3 rnd_vel = { vel_rand_range, vel_rand_range, vel_rand_range };
		XMFLOAT3 vel = { 0, 0, velocity_Z };
		vel.x += (float)rand() / RAND_MAX * rnd_vel.x - rnd_vel.x / 2.0f;
		vel.y += (float)rand() / RAND_MAX * rnd_vel.y - rnd_vel.y / 2.0f;
		vel.z += (float)rand() / RAND_MAX * rnd_vel.z - rnd_vel.z / 2.0f;

		const XMFLOAT3 rnd_acc = { accel_rand_range, accel_rand_range, accel_rand_range };
		XMFLOAT3 acc{};
		acc.x = (float)rand() / RAND_MAX * rnd_acc.x - rnd_acc.x / 2.0f;
		acc.y = (float)rand() / RAND_MAX * rnd_acc.y - rnd_acc.y / 2.0f;
		acc.z = (float)rand() / RAND_MAX * rnd_acc.z - rnd_acc.z / 2.0f;

		//カラー
		XMFLOAT4 s_color = start_color;
		XMFLOAT4 e_color = end_color;

		// 追加
		ParticleManager::GetInstance()->Add(life, position, vel, acc, 1.0f, 0.0f, s_color, e_color);
	}
}