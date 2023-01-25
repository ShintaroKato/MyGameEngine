#pragma once
#include "ParticleManager.h"

using namespace DirectX;

class ParticleEmitter
{
public:
	/// <summary>
	/// �S�����ɔ�΂�
	/// </summary>
	/// <param name="count">�����ɔ������鐔</param>
	/// <param name="life">�������鎞��</param>
	/// <param name="position">��������ʒu</param>
	/// <param name="velocity">���x</param>
	/// <param name="start_scale">�F(�J�n��)</param>
	/// <param name="end_scale">�F(�I����)</param>
	/// <param name="vel_rand_range">���x�̐U�ꕝ</param>
	/// <param name="accel_rand_range">�����x�̐U�ꕝ</param>
	/// <param name="start_scale">�傫��(�J�n��)</param>
	/// <param name="end_scale">�傫��(�I����)</param>
	static void EmitAllRange(
		unsigned int count, unsigned int life, XMFLOAT3 position, XMFLOAT3 velocity, 
		XMFLOAT4 start_color = { 1,1,1,1 }, XMFLOAT4 end_color = { 1,1,1,0 },
		float vel_rand_range = 0, float accel_rand_range = 0,
		float start_scale = 1.0f, float end_scale = 0.0f);

	/// <summary>
	/// X�������ɔ�΂�
	/// </summary>
	/// <param name="life">�������鎞��</param>
	/// <param name="position">��������ʒu</param>
	/// <param name="velocity">���x</param>
	/// <param name="start_scale">�F(�J�n��)</param>
	/// <param name="end_scale">�F(�I����)</param>
	/// <param name="vel_rand_range">���x�̐U�ꕝ</param>
	/// <param name="accel_rand_range">�����x�̐U�ꕝ</param>
	/// <param name="start_scale">�傫��(�J�n��)</param>
	/// <param name="end_scale">�傫��(�I����)</param>
	static void EmitX_AxisDir(
		unsigned int count, unsigned int life, XMFLOAT3 position, float velocity_X,
		XMFLOAT4 start_color = { 1,1,1,1 }, XMFLOAT4 end_color = { 1,1,1,0 },
		float vel_rand_range = 0, float accel_rand_range = 0,
		float start_scale = 1.0f, float end_scale = 0.0f);

	/// <summary>
	/// Y�������ɔ�΂�
	/// </summary>
	/// <param name="life">�������鎞��</param>
	/// <param name="position">��������ʒu</param>
	/// <param name="velocity">���x</param>
	/// <param name="start_scale">�F(�J�n��)</param>
	/// <param name="end_scale">�F(�I����)</param>
	/// <param name="vel_rand_range">���x�̐U�ꕝ</param>
	/// <param name="accel_rand_range">�����x�̐U�ꕝ</param>
	/// <param name="start_scale">�傫��(�J�n��)</param>
	/// <param name="end_scale">�傫��(�I����)</param>
	static void EmitY_AxisDir(
		unsigned int count, unsigned int life, XMFLOAT3 position, float velocity_Y,
		XMFLOAT4 start_color = { 1,1,1,1 }, XMFLOAT4 end_color = { 1,1,1,0 },
		float vel_rand_range = 0, float accel_rand_range = 0,
		float start_scale = 1.0f, float end_scale = 0.0f);

	/// <summary>
	/// Z�������ɔ�΂�
	/// </summary>
	/// <param name="life">�������鎞��</param>
	/// <param name="position">��������ʒu</param>
	/// <param name="velocity">���x</param>
	/// <param name="start_scale">�F(�J�n��)</param>
	/// <param name="end_scale">�F(�I����)</param>
	/// <param name="vel_rand_range">���x�̐U�ꕝ</param>
	/// <param name="accel_rand_range">�����x�̐U�ꕝ</param>
	/// <param name="start_scale">�傫��(�J�n��)</param>
	/// <param name="end_scale">�傫��(�I����)</param>
	static void EmitZ_AxisDir(
		unsigned int count, unsigned int life, XMFLOAT3 position, float velocity_Z,
		XMFLOAT4 start_color = { 1,1,1,1 }, XMFLOAT4 end_color = { 1,1,1,0 },
		float vel_rand_range = 0, float accel_rand_range = 0,
		float start_scale = 1.0f, float end_scale = 0.0f);
};