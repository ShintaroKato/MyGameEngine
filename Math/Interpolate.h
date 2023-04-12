#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Easing.h"

using namespace DirectX;

// 補間
class Interpolate
{
public:
	// スプライン
	static XMVECTOR Spline(const std::vector<XMVECTOR>& points, size_t startIndex, float t);
	static XMVECTOR Spline(const std::vector<XMFLOAT3>& points, size_t startIndex, float t);
};