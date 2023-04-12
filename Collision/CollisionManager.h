#pragma once
#include "CollisionPrimitive.h"
#include "RaycastHit.h"
#include "QueryCallback.h"

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

	/// <summary>
	/// ����̃R���C�_�[�ƁA����ȊO�̑S�ẴR���C�_�[�Ƃ̏Փ˂��`�F�b�N
	/// </summary>
	/// <param name="col">�R���C�_�[</param>
	bool CheckAllCollision(BaseCollider* col);

	/// <summary>
	/// ����̃R���C�_�[�ƁA�w�肵�����������S�ẴR���C�_�[�Ƃ̏Փ˂��`�F�b�N
	/// </summary>
	/// <param name="col">�R���C�_�[</param>
	/// <param name="attr">�����蔻�葮��</param>
	bool CheckAllCollision(BaseCollider* col, unsigned short attr);

	/// <summary>
	///	����̃R���C�_�[�ƁA�w�肵���R���C�_�[�������S�ẴR���C�_�[�Ƃ̏Փ˂��`�F�b�N
	/// </summary>
	/// <param name="col">�R���C�_�[</param>
	/// <param name="ex_col">���O����R���C�_�[</param>
	bool CheckAllCollision(BaseCollider* col, BaseCollider* ex_col);

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

	/// <summary>
	/// ���C�L���X�g
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="ex_attribute">���O����Փˑ���</param>
	/// <param name="ex_col">���O����R���C�_�[</param>
	/// <param name="hitInfo">�Փˏ��</param>
	/// <param name="maxDistance">�ő勗��</param>
	/// <returns>����</returns>
	bool Raycast(const Ray& ray, unsigned short ex_attribute, BaseCollider* ex_col, RaycastHit* hitInfo = nullptr,
		float maxDistance = D3D12_FLOAT32_MAX);

	/// <summary>
	/// ���ɂ��ՓˑS����
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="callback">�Փˎ��R�[���o�b�N</param>
	/// <param name="attribute">�Ώۂ̏Փˑ���</param>
	void QuerySphere(const Sphere& sphere, QueryCallback* callback, unsigned short attribute = (unsigned short)0xffff'ffff);

private:
	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	~CollisionManager() = default;
	CollisionManager& operator=(const CollisionManager&) = delete;
	//�R���C�_�[�̃��X�g
	std::forward_list<BaseCollider*>colliders;
};

