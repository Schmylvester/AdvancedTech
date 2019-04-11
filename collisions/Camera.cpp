#include "Camera.h"

Camera::Camera(float ratio)
{
	cam_pos = XMVectorSet(0.0f, 2.0f, -8, 0.0f);
	cam_up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	cam_target = XMVectorSet(0.0f, 0.0f, 0.5f, 0.0f);
	//cam_pos = XMVectorSet(0.0f, 500.0f, 0, 0.0f);
	//cam_up = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	//cam_target = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	cam_view = XMMatrixLookAtLH(cam_pos, cam_target, cam_up);
	cam_projection = XMMatrixPerspectiveFovLH(0.4f * 3.14f, ratio, 1.0f, 1000.0f);
}

XMMATRIX Camera::getWVPMatrix(XMMATRIX world_matrix)
{
	return world_matrix * cam_view * cam_projection;
}

void Camera::move(float x, float y, float z)
{
	cam_pos += XMVectorSet(x, y, z, 0);
	cam_target += XMVectorSet(x, y, z, 0);


	cam_view = XMMatrixLookAtLH(cam_pos, cam_target, cam_up);
}