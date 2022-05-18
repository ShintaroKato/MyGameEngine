#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>

// ノード
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

public:
	struct VertexPosNormalUv
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
	};

	Node* meshNode = nullptr;
	std::vector<VertexPosNormalUv> vertices;
	std::vector<unsigned short> indices;

private:
	// モデル名
	std::string name;
	// ノード配列
	std::vector<Node> node;
};