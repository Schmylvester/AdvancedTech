#pragma once
#include "ImageMapInfo.h"
class NavigationCell;
class NavMesh
{
public:
	NavMesh() = default;
	~NavMesh();
	NavigationCell* getClosestCell(Vector3 to) const;
	NavigationCell* getRandomCell(float min_y, float max_y) const;

	int cell_count;
	NavigationCell * cells = nullptr;
	bool initialised = false;
private:
};