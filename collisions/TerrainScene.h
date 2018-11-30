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
	void setGridNeighbours();

private:
	Terrain * active_cell;
	Geometry * player;
	std::vector<Geometry*> terrain;
	std::vector<Geometry*> safe_geometry;

	float move_speed = 17;
};

