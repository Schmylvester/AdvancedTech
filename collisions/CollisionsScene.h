#pragma once
#include "Scene.h"
#include "Terrain.h"

class CollisionsScene : public Scene
{
public:
	CollisionsScene() = default;
	~CollisionsScene();

	virtual void updateScene(float dt) override;
	virtual void drawScene(float dt) override;
	virtual void initObjects() override;
private:
	Geometry * player;
	
	float move_speed = 17;
};