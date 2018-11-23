#include "Camera.h"

Camera::Camera(float ratio)
{
	cam_pos = XMVectorSet(0.0f, 0.0f, z, 0.0f);
	cam_up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	cam_target = XMVectorSet(0.0f, 0.0f, 0.5f, 0.0f);

	cam_view = XMMatrixLookAtLH(cam_pos, cam_target, cam_up);
	cam_projection = XMMatrixPerspectiveFovLH(0.4f * 3.14f, ratio, 1.0f, 1000.0f);
}

XMMATRIX Camera::getWVPMatrix(XMMATRIX world_matrix)
{
	return world_matrix * cam_view * cam_projection;
}

void Camera::update(float dt)
{
	//z -= dt;
	//cam_pos = XMVectorSet(0.0f, 0.0f, z, 0.0f);
	//cam_view = XMMatrixLookAtLH(cam_pos, cam_target, cam_up);
}