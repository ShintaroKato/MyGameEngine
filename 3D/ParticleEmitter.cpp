#include "ParticleEmitter.h"

void ParticleEmitter::EmitRandomAllRange(unsigned int count, unsigned int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT4 start_color, XMFLOAT4 end_color, float vel_rand_range, float accel_rand_range, float start_scale, float end_scale)
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
		ParticleManager::GetInstance()->Add(life, position, vel, acc, start_scale, end_scale, s_color, e_color);
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
		ParticleManager::GetInstance()->Add(life, position, vel, acc, start_scale, end_scale, s_color, e_color);
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
		ParticleManager::GetInstance()->Add(life, position, vel, acc, start_scale, end_scale, s_color, e_color);
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
		ParticleManager::GetInstance()->Add(life, position, vel, acc, start_scale, end_scale, s_color, e_color);
	}
}

void ParticleEmitter::LaserBeam(unsigned int life, XMFLOAT3 start_position, XMFLOAT3 end_position, float velocity, XMFLOAT4 start_color, XMFLOAT4 end_color, float vel_rand_range, float accel_rand_range, float start_scale, float end_scale)
{
	for (int i = 0; i < 100; i++)
	{
		// 発射地点から終端までの距離
		XMFLOAT3 distance{};
		distance.x = end_position.x - start_position.x;
		distance.y = end_position.y - start_position.y;
		distance.z = end_position.z - start_position.z;

		// 発射地点から終端までの間にランダムに発生
		float rnd = (float)rand() / RAND_MAX;
		XMFLOAT3 position{};
		position.x = start_position.x + rnd * distance.x;
		position.y = start_position.y + rnd * distance.y;
		position.z = start_position.z + rnd * distance.z;

		// 発射する方向
		XMFLOAT3 direct{};
		direct.x = cos(atan2f(distance.z, distance.x));
		direct.z = sin(atan2f(distance.z, distance.x));
		direct.y = sin(atan2f(distance.y,
			sqrtf(distance.x * distance.x + distance.z * distance.z)));

		// X,Y,Z全て設定した数値内でランダムに分布
		const XMFLOAT3 rnd_vel = { vel_rand_range, vel_rand_range, vel_rand_range };
		XMFLOAT3 vel = direct;
		vel.x += (float)rand() / RAND_MAX * rnd_vel.x - rnd_vel.x / 2.0f;
		vel.y += (float)rand() / RAND_MAX * rnd_vel.y - rnd_vel.y / 2.0f;
		vel.z += (float)rand() / RAND_MAX * rnd_vel.z - rnd_vel.z / 2.0f;

		vel.x *= velocity;
		vel.y *= velocity;
		vel.z *= velocity;

		const XMFLOAT3 rnd_acc = { accel_rand_range, accel_rand_range, accel_rand_range };
		XMFLOAT3 acc{};
		acc.x = (float)rand() / RAND_MAX * rnd_acc.x - rnd_acc.x / 2.0f;
		acc.y = (float)rand() / RAND_MAX * rnd_acc.y - rnd_acc.y / 2.0f;
		acc.z = (float)rand() / RAND_MAX * rnd_acc.z - rnd_acc.z / 2.0f;

		//カラー
		XMFLOAT4 s_color = start_color;
		XMFLOAT4 e_color = end_color;

		// 追加
		ParticleManager::GetInstance()->Add(life, position, vel, acc, start_scale, end_scale, s_color, e_color);
	}
}
