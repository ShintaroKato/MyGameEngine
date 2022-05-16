#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>

// �m�[�h
struct Node
{
	std::string name;

	DirectX::XMVECTOR scaling = { 1,1,1,0 };
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	DirectX::XMVECTOR translation = { 0,0,0,1 };
	DirectX::XMMATRIX transform;
	DirectX::XMMATRIX globalTransform;

	Node* parent = nullptr;
};

class FBXModel
{
public:
	friend class FBXLoader;

private:
	// ���f����
	std::string name;
	// �m�[�h�z��
	std::vector<Node> node;
};