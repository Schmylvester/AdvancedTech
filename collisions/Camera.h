#pragma once
#include "Transform.h"
#include "DXUtil.h"

class Input;
class DXApp;
class Camera
{
public:
	Camera() = delete;
	Camera(Input* input, DXApp* _app);
	~Camera() = default;
	Transform m_transform;
	void tick(float dt);

	void rotate(XMFLOAT3 axis, float degrees);
	void move(float x, float y, float z);

	XMMATRIX getView() { return m_view_matrix; }
	XMMATRIX getPerspective() { return m_perspective_matrix; }

	void setLookTarget(Transform* set_to) { look_target = set_to; }

private:
	void changeRotation();
	XMFLOAT3 m_rotation_axis = XMFLOAT3(0, 0, 0);
	XMFLOAT3 m_up = XMFLOAT3(0, 1, 0);
	XMFLOAT3 m_target = XMFLOAT3(0, 0, 1);

	XMMATRIX m_view_matrix;
	XMMATRIX m_perspective_matrix;

	Transform* look_target = nullptr;

	Input* m_input;
	DXApp* m_app;
};