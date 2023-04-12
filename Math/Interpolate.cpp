#include "Interpolate.h"

XMVECTOR Interpolate::Spline(const std::vector<XMFLOAT3>& points, size_t startIndex, float t)
{
	std::vector<XMVECTOR> p;
	for (int i = 0; i < points.size(); i++)
	{
		p.push_back(XMLoadFloat3(&points[i]));
	}

	return Spline(p, startIndex, t);
}

XMVECTOR Interpolate::Spline(const std::vector<XMVECTOR>&points, size_t startIndex, float t)
{
	size_t n = points.size() - 2;

	if (startIndex >= n) return points[n]; // Pnの値を返す
	if (startIndex < 1) return points[1]; // P1の値を返す

	// P0〜P3の制御点を取得(P1〜P2を補間する)
	XMVECTOR p0 = points[startIndex - 1];
	XMVECTOR p1 = points[startIndex];
	XMVECTOR p2 = points[startIndex + 1];
	XMVECTOR p3 = points[startIndex + 2];

	// Catmull-Romの式による補間
	XMVECTOR position = 0.5 * (2 * p1 - (p0 - p2) * t +
		(2 * p0 - 5 * p1 + 4 * p2 - p3) * t * t -
		(p0 - 3 * p1 + 3 * p2 - p3) * t * t * t);

	return position;
}
