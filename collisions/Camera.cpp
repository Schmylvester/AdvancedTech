#include "Camera.h"

Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::tick(float dt)
{
	transform.update();
	transform.rotate('z', dt * 0.01f);
}
