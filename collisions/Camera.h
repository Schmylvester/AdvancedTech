#pragma once
#include "Transform.h"
#include "KeyboardInput.h"

class DXApp;
class Camera
{
public:
	Camera() = delete;
	Camera(KeyboardInput* input, DXApp* _app);
	~Camera();

	Transform m_transform;
	void tick(float dt);

private:
	DXApp * m_app;

	KeyboardInput * m_input;
};