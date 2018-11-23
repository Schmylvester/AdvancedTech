#pragma once
#include "DXApp.h"

class Scene : public DXApp
{
public:
	Scene() = default;
	~Scene();

	virtual void updateScene(float dt) override;
	virtual void drawScene(float dt) override;
	virtual void initObjects() override;

private:
	long frame_idx = 0;
	float last_dt[3] = { 0,0,0 };
	float player_speed = 0;
	float player_accel = 0.03f;
	bool player_dir = false;
	Geometry* plane;
	Geometry* player;
};