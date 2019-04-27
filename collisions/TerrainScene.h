#pragma once
#include "DXApp.h"

class Terrain;
class Player;
class TerrainScene : public DXApp
{
public:
	TerrainScene() = default;
	~TerrainScene();

	virtual void updateScene(float dt) override;
	virtual void drawObjects(float dt) override;
	virtual void initObjects() override;
	void setGridNeighbours();

private:
	Terrain * active_cell;
	Player * player;
	std::vector<Terrain*> terrain;
	std::vector<GameObject*> safe_geometry;
};

