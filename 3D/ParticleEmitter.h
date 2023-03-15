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
	static void EmitRandomAllRange(
		unsigned int count, unsigned int life, XMFLOAT3 position, XMFLOAT3 velocity, 
		XMFLOAT4 start_color = { 1,1,1,1 }, XMFLOAT4 end_color = { 1,1,1,0 },
		float vel_rand_range = 0, float accel_rand_range = 0,
		float start_scale = 1.0f, float end_scale = 0.0f);

	/// <summary>
	/// X�������ɔ�΂�
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
	static void EmitX_AxisDir(
		unsigned int count, unsigned int life, XMFLOAT3 position, float velocity_X,
		XMFLOAT4 start_color = { 1,1,1,1 }, XMFLOAT4 end_color = { 1,1,1,0 },
		float vel_rand_range = 0, float accel_rand_range = 0,
		float start_scale = 1.0f, float end_scale = 0.0f);

	/// <summary>
	/// Y�������ɔ�΂�
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
	static void EmitY_AxisDir(
		unsigned int count, unsigned int life, XMFLOAT3 position, float velocity_Y,
		XMFLOAT4 start_color = { 1,1,1,1 }, XMFLOAT4 end_color = { 1,1,1,0 },
		float vel_rand_range = 0, float accel_rand_range = 0,
		float start_scale = 1.0f, float end_scale = 0.0f);

	/// <summary>
	/// Z�������ɔ�΂�
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
	static void EmitZ_AxisDir(
		unsigned int count, unsigned int life, XMFLOAT3 position, float velocity_Z,
		XMFLOAT4 start_color = { 1,1,1,1 }, XMFLOAT4 end_color = { 1,1,1,0 },
		float vel_rand_range = 0, float accel_rand_range = 0,
		float start_scale = 1.0f, float end_scale = 0.0f);

	/// <summary>
	/// �r�[����ɔ�΂�
	/// </summary>
	/// <param name="count">�����ɔ������鐔</param>
	/// <param name="life">�������鎞��</param>
	/// <param name="start_position">��������ʒu</param>
	/// <param name="end_position">���e����ʒu</param>
	/// <param name="velocity">���x�̔{��</param>
	/// <param name="start_scale">�F(�J�n��)</param>
	/// <param name="end_scale">�F(�I����)</param>
	/// <param name="vel_rand_range">���x�̐U�ꕝ</param>
	/// <param name="accel_rand_range">�����x�̐U�ꕝ</param>
	/// <param name="start_scale">�傫��(�J�n��)</param>
	/// <param name="end_scale">�傫��(�I����)</param>
	static void LaserBeam(
		unsigned int count, unsigned int life, XMFLOAT3 start_position, XMFLOAT3 end_position, float velocity = 1.0f,
		XMFLOAT4 start_color = { 1,1,1,1 }, XMFLOAT4 end_color = { 1,1,1,0 },
		float vel_rand_range = 0, float accel_rand_range = 0,
		float start_scale = 1.0f, float end_scale = 5.0f);

	/// <summary>
	/// ��Ȃ̂悤�ɔ�΂�
	/// </summary>
	/// <param name="count">�����ɔ������鐔</param>
	/// <param name="node_count">�܂�Ȃ����</param>
	/// <param name="life">�������鎞��</param>
	/// <param name="start_position">��������ʒu</param>
	/// <param name="end_position">���e����ʒu</param>
	/// <param name="velocity">���x�̔{��</param>
	/// <param name="start_scale">�F(�J�n��)</param>
	/// <param name="end_scale">�F(�I����)</param>
	/// <param name="pos_rand_range">�ߖڂ̈ʒu�̐U�ꕝ</param>
	/// <param name="vel_rand_range">���x�̐U�ꕝ</param>
	/// <param name="accel_rand_range">�����x�̐U�ꕝ</param>
	/// <param name="start_scale">�傫��(�J�n��)</param>
	/// <param name="end_scale">�傫��(�I����)</param>
	static void Spark(
		unsigned int count, unsigned int node_count, unsigned int life, XMFLOAT3 start_position, XMFLOAT3 end_position, float velocity = 1.0f,
		XMFLOAT4 start_color = { 1,1,1,1 }, XMFLOAT4 end_color = { 1,1,1,0 },
		float pos_rand_range = 5.0f, float vel_rand_range = 0, float accel_rand_range = 0,
		float start_scale = 1.0f, float end_scale = 1.0f);

private:
	/// <summary>
	/// ��_�Ԃ̋������v�Z
	/// </summary>
	static XMFLOAT3 CalcDistance(XMFLOAT3 lhs, XMFLOAT3 rhs);
};