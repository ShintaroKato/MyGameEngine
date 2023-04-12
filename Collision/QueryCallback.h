#pragma once

#include <DirectXMath.h>

class ObjectOBJ;
class ObjectFBX;
class BaseCollider;

/// <summary>
/// クエリによる情報を得るための構造体
/// </summary>
struct QueryHit
{
	// 衝突相手のオブジェクト
	ObjectOBJ* obj = nullptr;
	ObjectFBX* fbx = nullptr;
	// 衝突相手のコライダー
	BaseCollider* collider = nullptr;
	// 衝突点
	DirectX::XMVECTOR inter;
	// 排斥ベクトル
	DirectX::XMVECTOR reject;
};

/// <summary>
/// クエリで交差を検出したときの動作を規定するクラス
/// </summary>
class QueryCallback
{
public:
	QueryCallback() = default;
	virtual ~QueryCallback() = default;

	/// <summary>
	/// 交差時コールバック
	/// </summary>
	/// <param name="info">交差情報</param>
	/// <returns>クエリを続けるならtrue, 打ち切るならfalse</returns>
	virtual bool OnQueryHit(const QueryHit& info) = 0;
};