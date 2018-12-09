#pragma once
#include <vector>

struct Path
{
	NavigationCell* previous_cell;
	NavigationCell* active_cell;
	float value;
};

class NavigationCell;
class Pathfinder
{
public:
	Pathfinder() = default;
	~Pathfinder() = default;

	std::vector<NavigationCell*> findpath(NavigationCell* from, NavigationCell* to);
private:
	bool listContains(NavigationCell* cell, bool check_open_list);
	void addToOpenList(Path add);
	float getDistance(NavigationCell* cell_a, NavigationCell* cell_b);
	NavigationCell* getPrev(NavigationCell* active);

	std::vector<Path> open_list;
	std::vector<Path> closed_list;

	NavigationCell* start_cell;
	NavigationCell* target_cell;
};