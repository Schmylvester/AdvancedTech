#include "Transform.h"

void Transform::translate(float x, float y, float z)
{
	position_matrix *= XMMatrixTranslation(x, y, z);
}

void Transform::translate(Vector3 move)
{
	translate(move.x, move.y, move.z);
}

void Transform::setPosition(float x, float y, float z)
{
	position_matrix = XMMatrixTranslation(x, y, z);
}

void Transform::setPosition(Vector3 pos)
{
	position_matrix = XMMatrixTranslation(pos.x, pos.y, pos.z);
}

void Transform::rotate(XMVECTOR axis, float angle)
{
	rotation_matrix *= XMMatrixRotationAxis(axis, angle);
}

void Transform::scale(float x, float y, float z)
{
	scale_matrix *= XMMatrixScaling(1 + x, 1 + y, 1 + z);
	scaleVector = Vector3(scaleVector.x * x, scaleVector.y * y, scaleVector.z * z);
}

const Vector4 Transform::getQuaternion()
{
	XMFLOAT4X4 rot;
	XMStoreFloat4x4(&rot, rotation_matrix);
	Vector4 q = Vector4();
	q.w = sqrt(max(0, 1 + rot._11 + rot._22 + rot._33)) / 2;
	q.x = sqrt(max(0, 1 + rot._11 - rot._22 - rot._33)) / 2;
	q.y = sqrt(max(0, 1 - rot._11 + rot._22 - rot._33)) / 2;
	q.z = sqrt(max(0, 1 - rot._11 - rot._22 + rot._33)) / 2;
	q.x *= (q.x * (rot._32 - rot._23)) < 0 ? -1 : 1;
	q.y *= (q.y * (rot._13 - rot._31)) < 0 ? -1 : 1;
	q.z *= (q.z * (rot._21 - rot._12)) < 0 ? -1 : 1;
	return q;
}

XMMATRIX Transform::getWorldMatrix()
{
	world_matrix = scale_matrix * rotation_matrix * position_matrix;
	return world_matrix;
}