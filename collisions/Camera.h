#pragma once
#include "Transform.h"
#include "Input.h"
#include "DXUtil.h"

class DXApp;
class Camera
{
public:
	Camera() = delete;
	Camera(Input* input, DXApp* _app);
	~Camera();
	Transform m_transform;
	void tick(float dt);

	XMMATRIX getView() { return m_view_matrix; }
	XMMATRIX getPerspective() { return m_perspective_matrix; }

	void setLookTarget(Transform* set_to) { look_target = set_to; }

private:
	XMMATRIX m_view_matrix;
	XMMATRIX m_perspective_matrix;

	Transform* look_target = nullptr;

	Input* m_input;
	DXApp* m_app;
};