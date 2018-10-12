#pragma once
#include <d3d11.h>
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;
class Transform
{
public:
	Transform() = default;
	~Transform() = default;

#pragma region GettersSetters
	Vector3 getPos();
	void move(float x, float y, float z);

	Vector3 getRot();
	void rotate(char axis, float rot);

	float getScl();

	DirectX::XMMATRIX* getMatrix()
	{
		return &world_matrix;
	}
#pragma endregion
	void update();

private:

	DirectX::XMMATRIX position_matrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX world_matrix = DirectX::XMMatrixIdentity();

	float scale = 0.05f;
};