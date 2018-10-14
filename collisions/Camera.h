#pragma once
#include "Transform.h"
#include "KeyboardInput.h"

class DXApp;
class Camera
{
public:
	Camera() = delete;
	Camera(KeyboardInput* input, DXApp* _app, DirectX::XMVECTOR look_at);
	~Camera();
	Transform m_transform;
	void tick(float dt);

private:
	KeyboardInput * m_input;
	DXApp* m_app;
};