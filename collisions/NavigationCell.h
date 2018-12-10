#pragma once
#include "DXUtil.h"
#include <vector>

class NavigationCell
{
public:
	static int cell_size;

	NavigationCell() = default;
	NavigationCell(int _x, float _y, int _z, XMFLOAT3 world);
	~NavigationCell() = default;

	bool checkNeighbour(NavigationCell* cell);
	void addNeighbour(NavigationCell* cell);
	Vector3 getIndexPos() const { return Vector3(x, y, z); }
	XMFLOAT3 getWorldPos() const { return world_pos; }
	bool getAccessible() const { return accessible; }
	void setAccessible(bool set) { accessible = set; }
	NavigationCell* getNeighbour(int i) const { return neighbours[i]; }
	int countNeighbours() const { return neighbour_count; }
private:
	static int climbable_incline;
	
	int getFreeNeighbour();

	int neighbour_count = 0;
	NavigationCell* neighbours[8] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	bool accessible = true;
	int x, z;
	float y;
	XMFLOAT3 world_pos;
};

