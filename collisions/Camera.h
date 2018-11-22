#pragma once
#include "DXUtil.h"

class Camera
{
public:
	Camera() = default;
	Camera(float ratio);
	~Camera() = default;

	XMMATRIX getWVPMatrix(XMMATRIX world_matrix);
	void update(float dt);
private:
	float x_pos = 0.0f;
	float y_pos = 0.0f;
	float z_pos = -1.5f;

	XMMATRIX cam_view;
	XMMATRIX cam_projection;

	XMVECTOR cam_pos;
	XMVECTOR cam_target;
	XMVECTOR cam_up;
};

