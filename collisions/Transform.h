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

	void setScl(float x, float y, float z);
	Vector3 getScl();

	DirectX::XMMATRIX* getMatrix();
	void setMatrix(DirectX::XMMATRIX new_matrix);
#pragma endregion
	void update();

private:

	DirectX::XMMATRIX position_matrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX world_matrix = DirectX::XMMatrixIdentity();

	Vector3 scale = Vector3(0.05f, 0.05f, 0.05f);
};