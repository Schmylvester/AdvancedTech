#include "NavMesh.h"
#include "NavigationCell.h"

NavMesh::~NavMesh()
{
	if (cells)
	{
		delete[] cells;
		cells = nullptr;
	}
}

NavigationCell* NavMesh::getClosestCell(Vector3 to) const
{
	float shortest_dist = INFINITY;
	int best_idx = -1;
	for (int i = 0; i < cell_count; i++)
	{
		float dist = Vector3::Distance(to, cells[i].getWorldPos());
		if (dist < shortest_dist)
		{
			shortest_dist = dist;
			best_idx = i;
		}
	}
	return &cells[best_idx];
}

NavigationCell * NavMesh::getRandomCell(float min_y, float max_y) const
{
	NavigationCell* return_cell = nullptr;
	int loop_break = 0;
	do
	{
		return_cell = &cells[rand() % cell_count];
	} while ((return_cell->getWorldPos().y < min_y
		|| return_cell->getWorldPos().y > max_y) 
		&& ++loop_break < 10000);
	assert(loop_break < 10000);
	return return_cell;
}
