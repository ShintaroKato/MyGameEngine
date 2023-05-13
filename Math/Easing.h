#pragma once
#include <DirectXMath.h>

using namespace DirectX;

// イージング
class Easing
{
public:
	static XMFLOAT3 EaseIn(float timeNow, XMFLOAT3 start, XMFLOAT3 end)
	{
		float time = timeNow * timeNow;
		time /= 60;

		XMFLOAT3 result{};
		result.x = start.x * (1.0f - time) + end.x * time;
		result.y = start.y * (1.0f - time) + end.y * time;
		result.z = start.z * (1.0f - time) + end.z * time;

		return result;
	}
	static XMFLOAT3 EaseOut(int timeNow, XMFLOAT3 start, XMFLOAT3 end)
	{
		float time = timeNow * (2 - timeNow);
		time /= 60;

		XMFLOAT3 result{};
		result.x = start.x * (1.0f - time) + end.x * time;
		result.y = start.y * (1.0f - time) + end.y * time;
		result.z = start.z * (1.0f - time) + end.z * time;

		return result;
	}
	static XMFLOAT3 EaseInOut(int timeNow, XMFLOAT3 start, XMFLOAT3 end)
	{
		float time = timeNow * timeNow * (3 - 2 * timeNow);
		time /= 60;

		XMFLOAT3 result{};
		result.x = start.x * (1.0f - time) + end.x * time;
		result.y = start.y * (1.0f - time) + end.y * time;
		result.z = start.z * (1.0f - time) + end.z * time;

		return result;
	}
};