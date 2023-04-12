#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Easing.h"

using namespace DirectX;

// •âŠÔ
class Interpolate
{
public:
	// ƒXƒvƒ‰ƒCƒ“
	static XMVECTOR Spline(const std::vector<XMVECTOR>& points, size_t startIndex, float t);
	static XMVECTOR Spline(const std::vector<XMFLOAT3>& points, size_t startIndex, float t);
};