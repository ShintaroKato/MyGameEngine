#pragma once
#include "ParticleManager.h"

using namespace DirectX;

class ParticleEmitter
{
public:
	/// <summary>
	/// 全方向に飛ばす
	/// </summary>
	/// <param name="count">同時に発生する数</param>
	/// <param name="life">生存する時間</param>
	/// <param name="position">発生する位置</param>
	/// <param name="velocity">速度</param>
	/// <param name="start_scale">色(開始時)</param>
	/// <param name="end_scale">色(終了時)</param>
	/// <param name="vel_rand_range">速度の振れ幅</param>
	/// <param name="accel_rand_range">加速度の振れ幅</param>
	/// <param name="start_scale">大きさ(開始時)</param>
	/// <param name="end_scale">大きさ(終了時)</param>
	static void EmitRandomAllRange(
		unsigned int count, unsigned int life, XMFLOAT3 position, XMFLOAT3 velocity, 
		XMFLOAT4 start_color = { 1,1,1,1 }, XMFLOAT4 end_color = { 1,1,1,0 },
		float vel_rand_range = 0, float accel_rand_range = 0,
		float start_scale = 1.0f, float end_scale = 0.0f);

	/// <summary>
	/// X軸方向に飛ばす
	/// </summary>
	/// <param name="count">同時に発生する数</param>
	/// <param name="life">生存する時間</param>
	/// <param name="position">発生する位置</param>
	/// <param name="velocity">速度</param>
	/// <param name="start_scale">色(開始時)</param>
	/// <param name="end_scale">色(終了時)</param>
	/// <param name="vel_rand_range">速度の振れ幅</param>
	/// <param name="accel_rand_range">加速度の振れ幅</param>
	/// <param name="start_scale">大きさ(開始時)</param>
	/// <param name="end_scale">大きさ(終了時)</param>
	static void EmitX_AxisDir(
		unsigned int count, unsigned int life, XMFLOAT3 position, float velocity_X,
		XMFLOAT4 start_color = { 1,1,1,1 }, XMFLOAT4 end_color = { 1,1,1,0 },
		float vel_rand_range = 0, float accel_rand_range = 0,
		float start_scale = 1.0f, float end_scale = 0.0f);

	/// <summary>
	/// Y軸方向に飛ばす
	/// </summary>
	/// <param name="count">同時に発生する数</param>
	/// <param name="life">生存する時間</param>
	/// <param name="position">発生する位置</param>
	/// <param name="velocity">速度</param>
	/// <param name="start_scale">色(開始時)</param>
	/// <param name="end_scale">色(終了時)</param>
	/// <param name="vel_rand_range">速度の振れ幅</param>
	/// <param name="accel_rand_range">加速度の振れ幅</param>
	/// <param name="start_scale">大きさ(開始時)</param>
	/// <param name="end_scale">大きさ(終了時)</param>
	static void EmitY_AxisDir(
		unsigned int count, unsigned int life, XMFLOAT3 position, float velocity_Y,
		XMFLOAT4 start_color = { 1,1,1,1 }, XMFLOAT4 end_color = { 1,1,1,0 },
		float vel_rand_range = 0, float accel_rand_range = 0,
		float start_scale = 1.0f, float end_scale = 0.0f);

	/// <summary>
	/// Z軸方向に飛ばす
	/// </summary>
	/// <param name="count">同時に発生する数</param>
	/// <param name="life">生存する時間</param>
	/// <param name="position">発生する位置</param>
	/// <param name="velocity">速度</param>
	/// <param name="start_scale">色(開始時)</param>
	/// <param name="end_scale">色(終了時)</param>
	/// <param name="vel_rand_range">速度の振れ幅</param>
	/// <param name="accel_rand_range">加速度の振れ幅</param>
	/// <param name="start_scale">大きさ(開始時)</param>
	/// <param name="end_scale">大きさ(終了時)</param>
	static void EmitZ_AxisDir(
		unsigned int count, unsigned int life, XMFLOAT3 position, float velocity_Z,
		XMFLOAT4 start_color = { 1,1,1,1 }, XMFLOAT4 end_color = { 1,1,1,0 },
		float vel_rand_range = 0, float accel_rand_range = 0,
		float start_scale = 1.0f, float end_scale = 0.0f);

	/// <summary>
	/// ビーム状に飛ばす
	/// </summary>
	/// <param name="count">同時に発生する数</param>
	/// <param name="life">生存する時間</param>
	/// <param name="start_position">発生する位置</param>
	/// <param name="end_position">着弾する位置</param>
	/// <param name="velocity">速度の倍率</param>
	/// <param name="start_scale">色(開始時)</param>
	/// <param name="end_scale">色(終了時)</param>
	/// <param name="vel_rand_range">速度の振れ幅</param>
	/// <param name="accel_rand_range">加速度の振れ幅</param>
	/// <param name="start_scale">大きさ(開始時)</param>
	/// <param name="end_scale">大きさ(終了時)</param>
	static void LaserBeam(
		unsigned int count, unsigned int life, XMFLOAT3 start_position, XMFLOAT3 end_position, float velocity = 1.0f,
		XMFLOAT4 start_color = { 1,1,1,1 }, XMFLOAT4 end_color = { 1,1,1,0 },
		float vel_rand_range = 0, float accel_rand_range = 0,
		float start_scale = 1.0f, float end_scale = 5.0f);

	/// <summary>
	/// 稲妻のように飛ばす
	/// </summary>
	/// <param name="count">同時に発生する数</param>
	/// <param name="node_count">折り曲げる回数</param>
	/// <param name="life">生存する時間</param>
	/// <param name="start_position">発生する位置</param>
	/// <param name="end_position">着弾する位置</param>
	/// <param name="velocity">速度の倍率</param>
	/// <param name="start_scale">色(開始時)</param>
	/// <param name="end_scale">色(終了時)</param>
	/// <param name="pos_rand_range">節目の位置の振れ幅</param>
	/// <param name="vel_rand_range">速度の振れ幅</param>
	/// <param name="accel_rand_range">加速度の振れ幅</param>
	/// <param name="start_scale">大きさ(開始時)</param>
	/// <param name="end_scale">大きさ(終了時)</param>
	static void Spark(
		unsigned int count, unsigned int node_count, unsigned int life, XMFLOAT3 start_position, XMFLOAT3 end_position, float velocity = 1.0f,
		XMFLOAT4 start_color = { 1,1,1,1 }, XMFLOAT4 end_color = { 1,1,1,0 },
		float pos_rand_range = 5.0f, float vel_rand_range = 0, float accel_rand_range = 0,
		float start_scale = 1.0f, float end_scale = 1.0f);

private:
	/// <summary>
	/// 二点間の距離を計算
	/// </summary>
	static XMFLOAT3 CalcDistance(XMFLOAT3 lhs, XMFLOAT3 rhs);
};