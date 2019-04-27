#pragma once
#include "DXApp.h"

class GameObject;
class Scene : public DXApp
{
public:
	Scene() = default;
	~Scene();

	virtual void updateScene(float dt) override;
	virtual void drawObjects(float dt) override;
	virtual void initObjects() override;

private:
	int frame_count = 0;

	float last_dt = 0;
	float max_dt = 0;
	float player_acc = 4.3f;
	float player_speed = 0;
	bool player_dir = false;
	GameObject* terrain;
	GameObject* player;
};