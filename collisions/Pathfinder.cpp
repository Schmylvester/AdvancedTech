#include "Pathfinder.h"
#include "NavigationCell.h"

std::vector<NavigationCell*> Pathfinder::findpath(NavigationCell * from, NavigationCell * to)
{
	start_cell = from;
	target_cell = to;

	//initialise the open list
	Path open;
	open.active_cell = from;
	open.previous_cell = nullptr;
	open.value = getDistance(open.active_cell, target_cell);
	open.value += getDistance(open.active_cell, start_cell);
	addToOpenList(open);

	bool path_found = false;
	int iter = 0;
	while (!path_found && iter++ < 10000)
	{
		//path found, hooray
		if (open.active_cell == target_cell)
		{
			path_found = true;
		}
		else
		{
			//check the neighbours of the first element of the open list
			for (NavigationCell* cell : open_list[0].active_cell->getNeighbours())
			{
				if (!listContains(cell, true) && !listContains(cell, false))
				{
					//set a new working candidate
					open.previous_cell = open.active_cell;
					open.active_cell = cell;
					open.value = getDistance(open.active_cell, target_cell);
					open.value += getDistance(open.active_cell, start_cell);
					addToOpenList(open);
				}
			}
			//remove it from the open list and add it to the closed list
			closed_list.push_back(open_list[0]);
			open_list.erase(open_list.begin());
		}
	}
	if (iter >= 10000)
	{
		OutputDebugString("Unable to find path");
		assert(false);
		return std::vector<NavigationCell*>();
	}
	//map out the path
	std::vector<NavigationCell*> return_path;
	return_path.push_back(open.active_cell);
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

//finds where on the open list this element should go
void Pathfinder::addToOpenList(Path add)
{
	std::vector<Path> temp_list;
	int i = 0;
	for (; i < open_list.size(); i++)
	{
		if (open_list[i].value < add.value)
		{
			temp_list.push_back(open_list[i]);
		}
	}
	temp_list.push_back(add);
	for (; i < open_list.size(); i++)
	{
		temp_list.push_back(open_list[i]);
	}
	open_list = temp_list;
}

//straight line distance between two cells
float Pathfinder::getDistance(NavigationCell * cell_a, NavigationCell * cell_b)
{
	int x = cell_a->getPos().x - cell_b->getPos().x;
	int z = cell_a->getPos().z - cell_b->getPos().z;
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
