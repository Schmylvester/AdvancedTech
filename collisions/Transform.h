#pragma once
#include "DXUtil.h"

class Transform
{
public:
	Transform() = default;
	~Transform() = default;

	void translate(float x, float y, float z);
	void translate(Vector3 move);
	void setPosition(float x, float y, float z);
	void rotate(XMVECTOR axis, float angle);
	void scale(float x, float y, float z);
	const Vector3 getPos() { return position_matrix.r[3]; }
	const Vector3 getScale() { return scaleVector; }
	const Vector4 getQuaternion();
	XMMATRIX getWorldMatrix();
private:
	XMMATRIX world_matrix;

	XMMATRIX position_matrix = XMMatrixIdentity();
	XMMATRIX rotation_matrix = XMMatrixIdentity();
	XMMATRIX scale_matrix = XMMatrixIdentity();

	Vector3 scaleVector = Vector3(1, 1, 1);
};
