#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Easing.h"

using namespace DirectX;

// ���
class Interpolate
{
public:
	// �X�v���C��
	static XMVECTOR Spline(const std::vector<XMVECTOR>& points, size_t startIndex, float t);
	static XMVECTOR Spline(const std::vector<XMFLOAT3>& points, size_t startIndex, float t);
};