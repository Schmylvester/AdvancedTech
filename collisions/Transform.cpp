#include "Transform.h"

void Transform::translate(float x, float y, float z)
{
	position_matrix *= XMMatrixTranslation(x, y, z);
}

void Transform::rotate(XMVECTOR axis, float angle)
{
	rotation_matrix *= XMMatrixRotationAxis(axis, angle);
}

void Transform::scale(float x, float y, float z)
{
	scale_matrix *= XMMatrixScaling(1 + x, 1 + y, 1 + z);
}

XMMATRIX Transform::getWorldMatrix()
{
	world_matrix = rotation_matrix * position_matrix * scale_matrix;
	return world_matrix;
}