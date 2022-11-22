#include "Collision.h"
#include <cmath>

bool Collision::CheckShpere2Sphere(const Sphere& sphere1, const Sphere& sphere2, XMVECTOR* inter, XMVECTOR* reject)
{
	float x = sphere2.center.m128_f32[0] - sphere1.center.m128_f32[0];
	float y = sphere2.center.m128_f32[1] - sphere1.center.m128_f32[1];
	float z = sphere2.center.m128_f32[2] - sphere1.center.m128_f32[2];
	float r = sphere1.radius + sphere2.radius;

	float dist = sqrtf(x * x + y * y + z * z);

	if (dist <= r)
	{
		if (inter)
		{
			*inter = (sphere2.center + sphere1.center) / 2;
		}

		// 押し出すベクトルを計算
		if (reject)
		{
			float rejectLen = r - sqrtf(dist);
			*reject = XMVector3Normalize(sphere1.center - sphere2.center);
			*reject *= rejectLen;
		}

		return true;
	}
	return false;
}

bool Collision::CheckSquare2Square(const Sphere::Square& square1, const Sphere::Square& square2, XMVECTOR* inter, XMVECTOR* reject)
{
	//１の下辺と２の上辺を比較
	if (square1.bottom > square2.top) return false;
	//１の上辺と２の下辺を比較
	if (square1.top < square2.bottom) return false;
	//１の左辺と２の右辺を比較
	if (square1.left > square2.right) return false;
	//１の右辺と２の左辺を比較
	if (square1.right < square2.left) return false;

	// 押し出し用のベクトルがなければ関数を終了
	if (!reject) return true;

	/*押し出し用のベクトルを計算*/

	// 各辺の長さ
	float verticalL1 = square1.top - square1.bottom;
	float horizontalL1 = square1.right - square1.left;

	float verticalL2 = square2.top - square2.bottom;
	float horizontalL2 = square2.right - square2.left;

	// 各辺の長さの合計
	float vLength = verticalL1 + verticalL2;
	float hLength = horizontalL1 + horizontalL2;

	// 辺が重なった部分の長さ
	// X <= Yになるように記録
	XMFLOAT2 vLRate{};
	XMFLOAT2 hLRate{};

	if (abs(square2.top - square1.bottom) <= abs(square1.top - square2.bottom))
	{// 上から接近
		vLRate = {
			square1.bottom - square2.top,
			square2.bottom - square1.top
		};
	}
	else
	{// 下から接近
		vLRate = {
			square1.top - square2.bottom,
			square2.top - square1.bottom
		};
	}

	if (abs(square2.right - square1.left) <= abs(square1.right - square2.left))
	{// 右から接近
		hLRate = {
			square1.left - square2.right,
			square2.left - square1.right
		};
	}
	else
	{// 左から接近
		hLRate = {
			square1.right - square2.left,
			square2.right - square1.left
		};
	}

	// 二辺の合計に対する辺が重なった部分の割合が小さい方に押し出す
	if (abs(hLRate.x) / hLength < abs(vLRate.x) / vLength)
	{
		reject->m128_f32[0] = hLRate.x;
		reject->m128_f32[2] = 0;
	}
	if (abs(hLRate.x) / hLength > abs(vLRate.x) / vLength)
	{
		reject->m128_f32[0] = 0;
		reject->m128_f32[2] = vLRate.x;
	}

	return true;
}

bool Collision::CheckShpere2Plane(const Sphere& sphere, const Plane& plane, XMVECTOR* inter)
{
	XMVECTOR distV = XMVector3Dot(sphere.center, plane.normal);
	float dist = distV.m128_f32[0] - plane.distance;

	if (fabsf(dist) > sphere.radius) return false;

	if (inter)
	{
		*inter = -dist * plane.normal + sphere.center;
	}

	return true;
}

void Collision::ClosestPtPoint2Triangle(const XMVECTOR& point, const Triangle& triangle, XMVECTOR* closest)
{
	// pointがp0の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	XMVECTOR p0_p2 = triangle.p2 - triangle.p0;
	XMVECTOR p0_pt = point - triangle.p0;

	XMVECTOR d1 = XMVector3Dot(p0_p1, p0_pt);
	XMVECTOR d2 = XMVector3Dot(p0_p2, p0_pt);

	if (d1.m128_f32[0] <= 0.0f && d2.m128_f32[0] <= 0.0f)
	{
		// p0が最近傍
		*closest = triangle.p0;
		return;
	}

	// pointがp1の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p1_pt = point - triangle.p1;

	XMVECTOR d3 = XMVector3Dot(p0_p1, p1_pt);
	XMVECTOR d4 = XMVector3Dot(p0_p2, p1_pt);

	if (d3.m128_f32[0] >= 0.0f && d4.m128_f32[0] <= d3.m128_f32[0])
	{
		// p1が最近傍
		*closest = triangle.p1;
		return;
	}

	// pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1.m128_f32[0] * d4.m128_f32[0] - d3.m128_f32[0] * d2.m128_f32[0];
	if (vc <= 0.0f && d1.m128_f32[0] >= 0.0f && d3.m128_f32[0] <= 0.0f)
	{
		float v = d1.m128_f32[0] / (d1.m128_f32[0] - d3.m128_f32[0]);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

	// pointがp2の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p2_pt = point - triangle.p2;

	XMVECTOR d5 = XMVector3Dot(p0_p1, p2_pt);
	XMVECTOR d6 = XMVector3Dot(p0_p2, p2_pt);
	if (d6.m128_f32[0] >= 0.0f && d5.m128_f32[0] <= d6.m128_f32[0])
	{
		*closest = triangle.p2;
		return;
	}

	// pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5.m128_f32[0] * d2.m128_f32[0] - d1.m128_f32[0] * d6.m128_f32[0];
	if (vb <= 0.0f && d2.m128_f32[0] >= 0.0f && d6.m128_f32[0] <= 0.0f)
	{
		float w = d2.m128_f32[0] / (d2.m128_f32[0] - d6.m128_f32[0]);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	// pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
	float va = d3.m128_f32[0] * d6.m128_f32[0] - d5.m128_f32[0] * d4.m128_f32[0];
	if (va <= 0.0f && (d4.m128_f32[0] - d3.m128_f32[0]) >= 0.0f && (d5.m128_f32[0] - d6.m128_f32[0]) >= 0.0f)
	{
		float w = (d4.m128_f32[0] - d3.m128_f32[0]) / ((d4.m128_f32[0] - d3.m128_f32[0]) + (d5.m128_f32[0] - d6.m128_f32[0]));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}

bool Collision::CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, XMVECTOR* inter, XMVECTOR* reject)
{
	XMVECTOR p;
	// 球の中心に対する最近接点である三角形上にある点ｐを見つける
	ClosestPtPoint2Triangle(sphere.center, triangle, &p);
	// 点ｐと球の中心の差分ベクトル
	XMVECTOR v = p - sphere.center;
	// 距離の二乗を求める
	// （同一ベクトル同士の内積　＝　三平方の定理の√内部の式）
	v = XMVector3Dot(v, v);
	// 球と三角形の距離が半径以下なら当っていない
	if (v.m128_f32[0] > sphere.radius * sphere.radius) 	return false;
	// 擬似交点を計算
	if (inter)
	{
		// 三角形上の最近接点ｐを擬似交点とする
		*inter = p;
	}

	// 押し出すベクトルを計算
	if (reject)
	{
		float ds = XMVector3Dot(sphere.center, triangle.normal).m128_f32[0];
		float dt = XMVector3Dot(triangle.p0, triangle.normal).m128_f32[0];
		float rejectLen = dt - ds + sphere.radius;
		*reject = triangle.normal * rejectLen;
	}

	return true;
}

bool Collision::ChackRay2Plane(const Ray& ray, const Plane& plane, float* distance, XMVECTOR* inter)
{
	const float epsilon = 1.0e-5f;//誤差吸収用の微小な値
	// 面法線とレイの方向ベクトルの内積
	float d1 = XMVector3Dot(plane.normal, ray.dir).m128_f32[0];
	// 裏面には当たらない
	if (d1 > -epsilon) { return false; }
	// 始点と原点の距離（平面の法線方向）
	// 面法線とレイの始点座標(位置ベクトル)の内積
	float d2 = XMVector3Dot(plane.normal, ray.start).m128_f32[0];
	// 始点と平面の距離（平面の法線方向）
	float dist = d2 - plane.distance;
	// 始点と平面の距離（レイ方向）
	float t = dist / -d1;
	// 交点が始点より後ろにあるので当たらない
	if (t < 0) { return false; }
	// 距離を書き込む
	if (distance) { *distance = t; }
	// 交点を計算
	if (inter) { *inter = ray.start + t * ray.dir; }

	return true;
}

bool Collision::CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance, XMVECTOR* inter)
{
	//三角形が乗っている平面を算出
	Plane plane;
	XMVECTOR interPlane;
	plane.normal = triangle.normal;
	plane.distance = XMVector3Dot(triangle.normal, triangle.p0).m128_f32[0];
	//レイと平面が当たっていなければ当たっていない
	if (!ChackRay2Plane(ray, plane, distance, &interPlane)) { return false; }
	//レイと平面の交点が三角形の内側にあるか判定
	const float epsilon = 1.0e-5f;
	XMVECTOR m;
	//辺p0_p1について
	XMVECTOR pt_p0 = triangle.p0 - interPlane;
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	m = XMVector3Cross(pt_p0, p0_p1);
	//辺の外側であれば当たっていないので判定を打ち切る
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) { return false; }

	//辺p1_p2について
	XMVECTOR pt_p1 = triangle.p1 - interPlane;
	XMVECTOR p1_p2 = triangle.p2 - triangle.p1;
	m = XMVector3Cross(pt_p1, p1_p2);
	//辺の外側であれば当たっていないので判定を打ち切る
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) { return false; }

	//辺p0_p1について
	XMVECTOR pt_p2 = triangle.p2 - interPlane;
	XMVECTOR p2_p0 = triangle.p0 - triangle.p2;
	m = XMVector3Cross(pt_p2, p2_p0);
	//辺の外側であれば当たっていないので判定を打ち切る
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) { return false; }

	//内側なので当たっている
	if (inter) { *inter = interPlane; }

	return true;
}

bool Collision::ChackRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance, XMVECTOR* inter)
{
	XMVECTOR m = ray.start - sphere.center;
	float b = XMVector3Dot(m, ray.dir).m128_f32[0];
	float c = XMVector3Dot(m, m).m128_f32[0] - sphere.radius * sphere.radius;
	//rayの始点がsphereの外側にあり(c > 0)、rayがsphereから離れていく方向を差している場合(b > 0)、当たらない
	if (c > 0.0f && b > 0.0f) { return false; }

	float discr = b * b - c;
	//負の判別式はレイが球を外れていることに一致
	if (discr < 0.0f) { return false; }

	//レイは球と交差している
	//交差する最小の値tを計算
	float t = -b - sqrtf(discr);
	//tが負である場合レイは球の内側から開始しているのでtを0にクランプ
	if (t < 0) { t = 0.0f; }
	if (distance) { *distance = t; }

	if (inter) { *inter = ray.start + t * ray.dir; }

	return true;
}
