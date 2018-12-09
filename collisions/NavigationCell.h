#pragma once
#include "DXUtil.h"
#include <vector>

class NavigationCell
{
public:
	NavigationCell() = default;
	~NavigationCell() = default;

	void checkNeighbour(NavigationCell* cell);
	Vector3 getPos() const { return Vector3(x, y, z); }
	bool getAccessible() const { return accessible; }
	std::vector<NavigationCell*> getNeighbours() const { return neighbours; }
private:
	static int climbable_incline;

	std::vector<NavigationCell*> neighbours;
	bool accessible = true;
	int x, y, z;
};

