#include "Transform.h"

#pragma region GettersSetters
Vector3 Transform::getPos()
{
	return position_matrix.r[3];
}
void Transform::move(float x, float y, float z)
{
	position_matrix *= DirectX::XMMatrixTranslation(x, y, z);
}
Vector3 Transform::getRot()
{
	return Vector3();
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
	scale = Vector3(x, y, z);
}
Vector3 Transform::getScl()
{
	return scale;
}
DirectX::XMMATRIX* Transform::getMatrix()
{
	return &world_matrix;
}
void Transform::setMatrix(DirectX::XMMATRIX new_matrix)
{
	view_matrix = new_matrix;
}
#pragma endregion
void Transform::update()
{
	world_matrix = position_matrix;
	world_matrix *= rotation_matrix;
	world_matrix *= view_matrix;
}
