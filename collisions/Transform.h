#pragma once
#include <d3d11.h>
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;
class Transform
{
public:
	Transform(float p_x = 0, float p_y = 0, float p_z = 0,
		float r_x = 0, float r_y = 0, float r_z = 0,
		float s_x = 0.05f, float s_y = 0.05f, float s_z = 0.05f);
	~Transform();

#pragma region GettersSetters
	Vector3 getPos();
	void setPos(Vector3 _position);

	Vector3 getRot();
	void setRot(Vector3 _rotation);

	Vector3 getScl();
	void setScl(Vector3 _scale);
#pragma endregion
#pragma region Operators
	bool operator ==(Transform t);
	bool operator !=(Transform t);
#pragma endregion

private:
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
};