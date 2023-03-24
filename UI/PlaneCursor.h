#pragma once
#include "ObjectOBJ.h"
#include "SphereCollider.h"

class PlaneCursor
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	PlaneCursor(){}

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	static void Initialize(ObjectOBJ* object);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	static void Update();

	/// <summary>
	/// 描画
	/// </summary>
	static void Draw();

	/// <summary>
	/// オブジェクトを掴んでいるか判定するフラグをセット
	/// </summary>
	static void SetIsDrag(bool isDrag) { PlaneCursor::isDrag = isDrag; }

	static bool GetIsDrag() { return isDrag; }

	static void SetRadius(float radius) { PlaneCursor::radius = radius; }

	static void SetSquareSideLength(float length_X, float length_Y)
	{
		PlaneCursor::sphereColl->SetSquareSideLength(length_X, length_Y);
	}

	/// <summary>
	/// 移動
	/// </summary>
	static void Move();

	static XMFLOAT3 GetPosition() { return pos; }

	static void SetPosition(const XMFLOAT3& position);

	static void SetColor(const XMFLOAT4& color) { cursor->SetColor(color); }

	static void SetMovableRange(const float& range) { movableRange = range; }

	static float GetMovableRange() { return movableRange; }
private:
	static ObjectOBJ* cursor;
	// 座標
	static XMFLOAT3 pos;
	// 半径
	static float radius;
	// 掴まれているか否か
	static bool isDrag;
	// 使用されているか否か
	static bool used;
	// ゲーム本編か否か
	static bool isInGame;
	// コライダー
	static Sphere sphere;
	static SphereCollider* sphereColl;
	// 動かせる範囲
	static float movableRange;
};