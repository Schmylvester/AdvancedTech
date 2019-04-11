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

const Vector3 Transform::getEulerAngles()
{
	XMFLOAT4X4 rot;
	XMStoreFloat4x4(&rot, rotation_matrix);
	float sy = sqrt(rot._11 * rot._11 + rot._21 * rot._21);

	bool singular = sy < 1e-6;

	float x, y, z;
	if (!singular)
	{
		x = atan2(rot._32, rot._33);
		y = atan2(-rot._31, sy);
		z = atan2(rot._21, rot._11);
	}
	else
	{
		x = atan2(-rot._23, rot._22);
		y = atan2(-rot._31, sy);
		z = 0;
	}
	return Vector3(x, y, z);
}

XMMATRIX Transform::getWorldMatrix()
{
	world_matrix = scale_matrix * rotation_matrix * position_matrix;
	return world_matrix;
}