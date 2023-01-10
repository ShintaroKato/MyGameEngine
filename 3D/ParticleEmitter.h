#pragma once
#include "ParticleManager.h"

using namespace DirectX;

class ParticleEmitter
{
	

	/// <summary>
	/// �S�����ɔ�΂�
	/// </summary>
	/// <param name="life">�������鎞��</param>
	/// <param name="velocity">���x</param>
	/// <param name="vel_rand_range">���x�̐U�ꕝ</param>
	/// <param name="accel_rand_range">�����x�̐U�ꕝ</param>
	/// <param name="start_scale">�傫��(�J�n��)</param>
	/// <param name="end_scale">�傫��(�I����)</param>
	/// <param name="start_scale">�F(�J�n��)</param>
	/// <param name="end_scale">�F(�I����)</param>
	void EmitAllRange(int life, float velocity, float vel_rand_range, float accel_rand_range, float start_scale, float end_scale, XMFLOAT4 start_color, XMFLOAT4 end_color);

	/// <summary>
	/// X�������ɔ�΂�
	/// </summary>
	/// <param name="life">�������鎞��</param>
	/// <param name="velocity">���x</param>
	/// <param name="vel_rand_range">���x�̐U�ꕝ</param>
	/// <param name="accel_rand_range">�����x�̐U�ꕝ</param>
	/// <param name="start_scale">�傫��(�J�n��)</param>
	/// <param name="end_scale">�傫��(�I����)</param>
	/// <param name="start_scale">�F(�J�n��)</param>
	/// <param name="end_scale">�F(�I����)</param>
	void EmitX_AxisDir(int life, float velocity_X, float vel_rand_range, float accel_rand_range, float start_scale, float end_scale, XMFLOAT4 start_color, XMFLOAT4 end_color);

	/// <summary>
	/// Y�������ɔ�΂�
	/// </summary>
	/// <param name="life">�������鎞��</param>
	/// <param name="velocity">���x</param>
	/// <param name="vel_rand_range">���x�̐U�ꕝ</param>
	/// <param name="accel_rand_range">�����x�̐U�ꕝ</param>
	/// <param name="start_scale">�傫��(�J�n��)</param>
	/// <param name="end_scale">�傫��(�I����)</param>
	/// <param name="start_scale">�F(�J�n��)</param>
	/// <param name="end_scale">�F(�I����)</param>
	void EmitY_AxisDir(int life, float velocity_Y, float vel_rand_range, float accel_rand_range, float start_scale, float end_scale, XMFLOAT4 start_color, XMFLOAT4 end_color);

	/// <summary>
	/// Z�������ɔ�΂�
	/// </summary>
	/// <param name="life">�������鎞��</param>
	/// <param name="velocity">���x</param>
	/// <param name="vel_rand_range">���x�̐U�ꕝ</param>
	/// <param name="accel_rand_range">�����x�̐U�ꕝ</param>
	/// <param name="start_scale">�傫��(�J�n��)</param>
	/// <param name="end_scale">�傫��(�I����)</param>
	/// <param name="start_scale">�F(�J�n��)</param>
	/// <param name="end_scale">�F(�I����)</param>
	void EmitZ_AxisDir(int life, float velocity_Z, float vel_rand_range, float accel_rand_range, float start_scale, float end_scale, XMFLOAT4 start_color, XMFLOAT4 end_color);

private:
	XMFLOAT3 position{};
};