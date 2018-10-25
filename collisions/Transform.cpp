#include "Transform.h"

#pragma region GettersSetters
SimpleMath::Vector3 Transform::getPos()
{
	return position_matrix.r[3];
}
void Transform::move(float x, float y, float z)
{
	position_matrix *= DirectX::XMMatrixTranslation(x, y, z);
}

void Transform::rotate(char axis, float rot)
{
	switch (axis)
	{
	case 'x':
		rotation_matrix *= DirectX::XMMatrixRotationX(rot);
		break;
	case 'y':
		rotation_matrix *= DirectX::XMMatrixRotationY(rot);
		break;
	case 'z':
		rotation_matrix *= DirectX::XMMatrixRotationZ(rot);
		break;
	}
}
void Transform::setScl(float x, float y, float z)
{
	scale_matrix *= SimpleMath::Vector3(x, y, z);
}

SimpleMath::Vector3 Transform::getScale()
{
	return scale_matrix;
}

DirectX::XMMATRIX* Transform::getMatrix()
{
	return &world_matrix;
}
#pragma endregion
void Transform::update()
{
	world_matrix = rotation_matrix;
	world_matrix *= position_matrix;
}
