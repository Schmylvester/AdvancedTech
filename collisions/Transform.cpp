#include "Transform.h"

void Transform::translate(float x, float y, float z)
{
	position_matrix *= XMMatrixTranslation(x, y, z);
}

void Transform::setPosition(float x, float y, float z)
{
	position_matrix = XMMatrixTranslation(x, y, z);
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
	float w = sqrt(1.0 + rot._11 + rot._22 + rot._33) / 2.0;
	double w4 = (4.0 * w);
	float x = (rot._32 - rot._23) / w4;
	float y = (rot._13 - rot._31) / w4;
	float z = (rot._21 - rot._12) / w4;

	return Vector4(x, y, z, w);
}

XMMATRIX Transform::getWorldMatrix()
{
	world_matrix = scale_matrix * rotation_matrix * position_matrix;
	return world_matrix;
}