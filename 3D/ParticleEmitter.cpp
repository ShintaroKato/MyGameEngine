#include "ParticleEmitter.h"
#define PI 3.141592;

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

void ParticleEmitter::LaserBeam(unsigned int count, unsigned int life, XMFLOAT3 start_position, XMFLOAT3 end_position, float velocity, XMFLOAT4 start_color, XMFLOAT4 end_color, float vel_rand_range, float accel_rand_range, float start_scale, float end_scale)
{
	for (int i = 0; i < count; i++)
	{
		// 発射地点から終端までの距離
		XMFLOAT3 distance = CalcDistance(end_position, start_position);

		// 発射地点から終端までの間の直線上に発生
		XMFLOAT3 position{};
		position.x = start_position.x + distance.x / count * i;
		position.y = start_position.y + distance.y / count * i;
		position.z = start_position.z + distance.z / count * i;

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

void ParticleEmitter::Spark(unsigned int count, unsigned int node_count, unsigned int life, XMFLOAT3 start_position, XMFLOAT3 end_position, float velocity, XMFLOAT4 start_color, XMFLOAT4 end_color, float pos_rand_range, float vel_rand_range, float accel_rand_range, float start_scale, float end_scale)
{
	XMFLOAT3 start = start_position;
	XMFLOAT3 distance = CalcDistance(end_position, start_position);

	for (int i = 1; i <= node_count; i++)
	{
		//現在の始点からランダムでずらして節目の座標を設定
		XMFLOAT3 node = start;
		node.x += distance.x / node_count + (float)rand() / RAND_MAX * pos_rand_range - pos_rand_range / 2;
		node.y += distance.y / node_count + (float)rand() / RAND_MAX * pos_rand_range - pos_rand_range / 2;
		node.z += distance.z / node_count + (float)rand() / RAND_MAX * pos_rand_range - pos_rand_range / 2;

		if(i == node_count) node = end_position;

		ParticleEmitter::LaserBeam(count, life, start, node, velocity,
			start_color, end_color, vel_rand_range, accel_rand_range, start_scale, end_scale);

		// 現在の終端を次の始点にする
		start = node;
	}
}

void ParticleEmitter::CircleXZ(unsigned int count, unsigned int life, XMFLOAT3 position, float radius, float velocity, float accel, XMFLOAT4 start_color, XMFLOAT4 end_color, float start_scale, float end_scale)
{
	if (count <= 0) return;

	float angle = 360.0f / count;
	float radian = XMConvertToRadians(angle);

	for (int i = 0; i < count; i++)
	{
		XMFLOAT3 startPos = position;
		startPos.x += radius * cosf(radian * i);
		startPos.z += radius * sinf(radian * i);

		// 速度
		XMFLOAT3 vel = { velocity, 0, velocity };
		vel.x *= cosf(radian * i);
		vel.y = 0;
		vel.z *= sinf(radian * i);

		// 加速度
		XMFLOAT3 acc = { accel, 0, accel };
		acc.x *= cosf(radian * i);
		acc.y = 0;
		acc.z *= sinf(radian * i);

		//カラー
		XMFLOAT4 s_color = start_color;
		XMFLOAT4 e_color = end_color;

		// 追加
		ParticleManager::GetInstance()->Add(life, position, vel, acc, start_scale, end_scale, s_color, e_color);
	}
}

XMFLOAT3 ParticleEmitter::CalcDistance(XMFLOAT3 lhs, XMFLOAT3 rhs)
{
	XMFLOAT3 result{};
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	result.z = lhs.z - rhs.z;

	return result;
}
