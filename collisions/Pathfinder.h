#pragma once
#include <vector>

class NavigationCell;
struct Path
{
	NavigationCell* previous_cell;
	NavigationCell* active_cell;
	float value;
};

class Pathfinder
{
public:
	Pathfinder() = default;
	~Pathfinder() = default;

	std::vector<NavigationCell*> findpath(NavigationCell* from, NavigationCell* to) const;
private:
	//weighting in favour of the start cell or the end cell (1.0f for equal, higher for target, lower for origin)
	float skew = 1.0f;
	bool listContains(NavigationCell * cell, std::vector<Path>* list_to_check) const;
	float getDistance(NavigationCell* cell_a, NavigationCell* cell_b) const;
	NavigationCell * getPrev(NavigationCell * active, std::vector<Path>* open, std::vector<Path>* closed) const;
	int getBestCandidate(std::vector<Path>* open) const;
};