#pragma once
#include "ParticleManager.h"

using namespace DirectX;

class ParticleEmitter
{
	

	/// <summary>
	/// 全方向に飛ばす
	/// </summary>
	/// <param name="life">生存する時間</param>
	/// <param name="velocity">速度</param>
	/// <param name="vel_rand_range">速度の振れ幅</param>
	/// <param name="accel_rand_range">加速度の振れ幅</param>
	/// <param name="start_scale">大きさ(開始時)</param>
	/// <param name="end_scale">大きさ(終了時)</param>
	/// <param name="start_scale">色(開始時)</param>
	/// <param name="end_scale">色(終了時)</param>
	void EmitAllRange(int life, float velocity, float vel_rand_range, float accel_rand_range, float start_scale, float end_scale, XMFLOAT4 start_color, XMFLOAT4 end_color);

	/// <summary>
	/// X軸方向に飛ばす
	/// </summary>
	/// <param name="life">生存する時間</param>
	/// <param name="velocity">速度</param>
	/// <param name="vel_rand_range">速度の振れ幅</param>
	/// <param name="accel_rand_range">加速度の振れ幅</param>
	/// <param name="start_scale">大きさ(開始時)</param>
	/// <param name="end_scale">大きさ(終了時)</param>
	/// <param name="start_scale">色(開始時)</param>
	/// <param name="end_scale">色(終了時)</param>
	void EmitX_AxisDir(int life, float velocity_X, float vel_rand_range, float accel_rand_range, float start_scale, float end_scale, XMFLOAT4 start_color, XMFLOAT4 end_color);

	/// <summary>
	/// Y軸方向に飛ばす
	/// </summary>
	/// <param name="life">生存する時間</param>
	/// <param name="velocity">速度</param>
	/// <param name="vel_rand_range">速度の振れ幅</param>
	/// <param name="accel_rand_range">加速度の振れ幅</param>
	/// <param name="start_scale">大きさ(開始時)</param>
	/// <param name="end_scale">大きさ(終了時)</param>
	/// <param name="start_scale">色(開始時)</param>
	/// <param name="end_scale">色(終了時)</param>
	void EmitY_AxisDir(int life, float velocity_Y, float vel_rand_range, float accel_rand_range, float start_scale, float end_scale, XMFLOAT4 start_color, XMFLOAT4 end_color);

	/// <summary>
	/// Z軸方向に飛ばす
	/// </summary>
	/// <param name="life">生存する時間</param>
	/// <param name="velocity">速度</param>
	/// <param name="vel_rand_range">速度の振れ幅</param>
	/// <param name="accel_rand_range">加速度の振れ幅</param>
	/// <param name="start_scale">大きさ(開始時)</param>
	/// <param name="end_scale">大きさ(終了時)</param>
	/// <param name="start_scale">色(開始時)</param>
	/// <param name="end_scale">色(終了時)</param>
	void EmitZ_AxisDir(int life, float velocity_Z, float vel_rand_range, float accel_rand_range, float start_scale, float end_scale, XMFLOAT4 start_color, XMFLOAT4 end_color);

private:
	XMFLOAT3 position{};
};