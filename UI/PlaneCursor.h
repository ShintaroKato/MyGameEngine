#pragma once
#include "ObjectOBJ.h"
#include "SphereCollider.h"

class PlaneCursor
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	PlaneCursor(){}

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	static void Initialize(ObjectOBJ* object);

	/// <summary>
	/// ���t���[������
	/// </summary>
	static void Update();

	/// <summary>
	/// �`��
	/// </summary>
	static void Draw();

	/// <summary>
	/// �I�u�W�F�N�g��͂�ł��邩���肷��t���O���Z�b�g
	/// </summary>
	static void SetIsDrag(bool isDrag) { PlaneCursor::isDrag = isDrag; }

	static bool GetIsDrag() { return isDrag; }

	static void SetRadius(float radius) { PlaneCursor::radius = radius; }

	static void SetSquareSideLength(float length_X, float length_Y)
	{
		PlaneCursor::sphereColl->SetSquareSideLength(length_X, length_Y);
	}

	/// <summary>
	/// �ړ�
	/// </summary>
	static void Move();

	static XMFLOAT3 GetPosition() { return pos; }

	static void SetPosition(const XMFLOAT3& position);

	static void SetColor(const XMFLOAT4& color) { cursor->SetColor(color); }

	static void SetMovableRange(const float& range) { movableRange = range; }

	static float GetMovableRange() { return movableRange; }
private:
	static ObjectOBJ* cursor;
	// ���W
	static XMFLOAT3 pos;
	// ���a
	static float radius;
	// �͂܂�Ă��邩�ۂ�
	static bool isDrag;
	// �g�p����Ă��邩�ۂ�
	static bool used;
	// �Q�[���{�҂��ۂ�
	static bool isInGame;
	// �R���C�_�[
	static Sphere sphere;
	static SphereCollider* sphereColl;
	// ��������͈�
	static float movableRange;
};