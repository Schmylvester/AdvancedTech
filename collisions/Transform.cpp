#include "Transform.h"

Transform::Transform(float p_x, float p_y, float p_z,
	float r_x, float r_y, float r_z, float s_x, float s_y, float s_z)
{
	position.x = p_x;
	position.y = p_y;
	position.z = p_z;
	rotation.x = r_x;
	rotation.y = r_y;
	rotation.z = r_z;
	scale.x = s_x;
	scale.y = s_y;
	scale.z = s_z;
}

Transform::~Transform()
{
}

#pragma region Operators
bool Transform::operator ==(Transform t)
{
	return position == t.position &&
		rotation == t.rotation &&
		scale == t.scale;
}
bool Transform::operator !=(Transform t)
{
	return !(*this == t);
}
#pragma endregion

#pragma region GettersSetters
Vector3 Transform::getPos()
{
	return position;
}
void Transform::setPos(Vector3 _position)
{
	position = _position;
}
Vector3 Transform::getRot()
{
	return rotation;
}
void Transform::setRot(Vector3 _rotation)
{
	rotation = _rotation;
}
Vector3 Transform::getScl()
{
	return scale;
}
void Transform::setScl(Vector3 _scale)
{
	scale = _scale;
}
#pragma endregion
