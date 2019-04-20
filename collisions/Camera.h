#pragma once
#include "DXUtil.h"

class Camera
{
public:
	Camera() = default;
	Camera(float ratio, bool top_down = false);
	~Camera() = default;

	XMMATRIX getWVPMatrix(XMMATRIX world_matrix);
	void move(float x, float y, float z);
	void setPos(float x, float y, float z);
	void lookAt(Vector3 target);
	void rotateAround(float angle);
private:
	XMMATRIX cam_view;
	XMMATRIX cam_projection;

	XMVECTOR cam_pos;
	XMVECTOR cam_target;
	XMVECTOR cam_up;
};

