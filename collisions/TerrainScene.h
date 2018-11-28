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
	Geometry * player;
	Terrain * terrain;

	float move_speed = 8;
};

