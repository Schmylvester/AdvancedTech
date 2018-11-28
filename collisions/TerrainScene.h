#pragma once
#include "DXApp.h"

class Terrain;
class TerrainScene : public DXApp
{
public:
	TerrainScene() = default;
	~TerrainScene();

	virtual void updateScene(float dt) override;
	virtual void drawScene(float dt) override;
	virtual void initObjects() override;

private:
	Terrain * active_cell;
	Geometry * player;
	std::vector<Geometry*> terrain;

	float move_speed = 20;
};

