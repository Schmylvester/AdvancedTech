#include "Pathfinder.h"
#include "NavigationCell.h"

std::vector<NavigationCell*> Pathfinder::findpath(NavigationCell * from, NavigationCell * to)
{
	if (!to->getAccessible())
	{
		OutputDebugString("You can not go there");
		return std::vector<NavigationCell*>();
	}
	start_cell = from;
	target_cell = to;

	//initialise the open list
	Path open;
	open.active_cell = from;
	open.previous_cell = nullptr;
	open.value = (getDistance(open.active_cell, target_cell) * 3);
	open.value += getDistance(open.active_cell, start_cell);
	open_list.push_back(open);

	bool path_found = false;
	int iter = 0;
	while (!path_found && ++iter < 70000)
	{
		int best_candidate = getBestCandidate();
		if (open_list.size() == 0)
		{
			OutputDebugString("Unable to find path, confident that one does not exist");
			return std::vector<NavigationCell*>();
		}
		//path found, hooray
		else if (open_list[best_candidate].active_cell == target_cell)
		{
			path_found = true;
		}
		else
		{
			//check the neighbours of the first element of the open list
			for (int i = 0; i < 8; i++)
			{
				NavigationCell* cell = open_list[best_candidate].active_cell->getNeighbour(i);
				if (cell != nullptr)
				{
					if (!listContains(cell, true) && !listContains(cell, false))
					{
						//set a new working candidate
						open.previous_cell = open_list[best_candidate].active_cell;
						open.active_cell = cell;
						open.value = getDistance(open.active_cell, target_cell);
						open.value += getDistance(open.active_cell, start_cell);
						open_list.push_back(open);
					}
				}
			}
			//remove it from the open list and add it to the closed list
			closed_list.push_back(open_list[best_candidate]);
			open_list.erase(open_list.begin() + best_candidate);
		}
	}
	if (iter >= 70000)
	{
		OutputDebugString("Unable to find path, there might be one but it would take ages to find");
		assert(false);
		return std::vector<NavigationCell*>();
	}
	//map out the path
	std::vector<NavigationCell*> return_path;
	return_path.push_back(target_cell);
	while (return_path.back() != start_cell)
	{
		return_path.push_back(getPrev(return_path.back()));
	}
	//it's backwards, so reverse it
	std::vector<NavigationCell*> temp_path = return_path;
	for (int i = return_path.size() - 1; i >= 0; i--)
	{
		return_path[i] = temp_path[return_path.size() - 1 - i];
	}

	return return_path;
}

//checks a list for an element
bool Pathfinder::listContains(NavigationCell * cell, bool check_open_list)
{
	if (check_open_list)
	{
		for (Path p : open_list)
		{
			if (p.active_cell == cell)
				return true;
		}
	}
	else
	{
		for (Path p : closed_list)
		{
			if (p.active_cell == cell)
				return true;
		}
	}
	return false;
}

//straight line distance between two cells
float Pathfinder::getDistance(NavigationCell * cell_a, NavigationCell * cell_b)
{
	int x = cell_a->getIndexPos().x - cell_b->getIndexPos().x;
	int z = cell_a->getIndexPos().z - cell_b->getIndexPos().z;
	float distance = sqrt(pow(x, 2) + pow(z, 2));
	return distance;
}

//get the cell that led to this cell in the chain
NavigationCell * Pathfinder::getPrev(NavigationCell * active)
{
	for (Path p : closed_list)
	{
		if (p.active_cell == active)
		{
			return p.previous_cell;
		}
	}
	for (Path p : open_list)
	{
		if (p.active_cell == active)
		{
			return p.previous_cell;
		}
	}

	OutputDebugString("Error in the pathfinding code");
	assert(false);
	return nullptr;
}

int Pathfinder::getBestCandidate() const
{
	int best_candidate = 0;

	for (int i = 1; i < open_list.size(); i++)
	{
		if (open_list[i].value < open_list[best_candidate].value)
		{
			best_candidate = i;
		}
	}

	return best_candidate;
}
