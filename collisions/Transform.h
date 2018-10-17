#pragma once
#include "DXUtil.h"
#include <SimpleMath.h>

class Transform
{
public:
	Transform() = default;
	~Transform() = default;

#pragma region GettersSetters
	SimpleMath::Vector3 getPos();
	void move(float x, float y, float z);

	DirectX::XMMATRIX* getRotMatrix() { return &rotation_matrix; }
	void rotate(char axis, float rot);

	void setScl(float x, float y, float z);

	SimpleMath::Vector3 getScale();

	DirectX::XMMATRIX* getMatrix();
#pragma endregion
	void update();

private:
	XMMATRIX position_matrix = XMMatrixIdentity();
	XMMATRIX rotation_matrix = XMMatrixIdentity();
	SimpleMath::Vector3 scale_matrix = SimpleMath::Vector3(0.05f, 0.05f, 0.05f);

	XMMATRIX world_matrix = XMMatrixIdentity();

};