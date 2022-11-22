#pragma once
#include "CollisionPrimitive.h"
#include "RaycastHit.h"

#include <d3d12.h>
#include <forward_list>
#include <algorithm>

class BaseCollider;

class CollisionManager
{
public: //�ÓI�����o�֐�
	static CollisionManager* GetInstance();

public: //�����o�֐�

	/// <summary>
	/// �R���C�_�[�̒ǉ�
	/// </summary>
	/// <param name="collider">�R���C�_�[</param>
	inline void AddCollider(BaseCollider* collider)
	{
		// ���X�g���̗v�f��T��
		auto result = std::find(colliders.begin(), colliders.end(), collider);
		// �w�肵���v�f��������Ȃ������烊�X�g�ɗv�f��ǉ�
		if(result == colliders.end()) colliders.push_front(collider);
	}

	/// <summary>
	/// �R���C�_�[�̍폜
	/// </summary>
	/// <param name="collider">�R���C�_�[</param>
	inline void RemoveCollider(BaseCollider* collider)
	{
		colliders.remove(collider);
	}

	/// <summary>
	/// �R���C�_�[��S�č폜
	/// </summary>
	inline void DeleteCollider()
	{
		colliders.clear();
	}

	/// <summary>
	/// �S�Ă̏Փ˃`�F�b�N
	/// </summary>
	bool CheckAllCollision();
	bool CheckAllCollision(BaseCollider* col);
	bool CheckAllCollision(BaseCollider* col, unsigned short attr);

	/// <summary>
	///
	/// </summary>
	/// <returns>����</returns>
	bool CheckCollision(BaseCollider* colA, BaseCollider* colB);

	/// <summary>
	/// �Z�b�g����Ă���I�u�W�F�N�g�𔻕�
	/// </summary>
	void CheckSetObject(BaseCollider* colA, BaseCollider* colB, XMVECTOR inter, XMVECTOR reject = {});

	/// <summary>
	/// ���C�L���X�g
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="hitInfo">�Փˏ��</param>
	/// <param name="maxDistance">�ő勗��</param>
	/// <returns>����</returns>
	bool Raycast(const Ray& ray, RaycastHit* hitInfo = nullptr,
		float maxDistance = D3D12_FLOAT32_MAX);

	/// <summary>
	/// ���C�L���X�g
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="attribute">�Ώۂ̏Փˑ���</param>
	/// <param name="hitInfo">�Փˏ��</param>
	/// <param name="maxDistance">�ő勗��</param>
	/// <returns>����</returns>
	bool Raycast(const Ray& ray, unsigned short attribute, RaycastHit* hitInfo = nullptr,
		float maxDistance = D3D12_FLOAT32_MAX);

private:
	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	~CollisionManager() = default;
	CollisionManager& operator=(const CollisionManager&) = delete;
	//�R���C�_�[�̃��X�g
	std::forward_list<BaseCollider*>colliders;
};

