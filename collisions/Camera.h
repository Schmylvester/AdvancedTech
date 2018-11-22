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
	XMMATRIX cam_view;
	XMMATRIX cam_projection;

	XMVECTOR cam_pos;
	XMVECTOR cam_target;
	XMVECTOR cam_up;
};

