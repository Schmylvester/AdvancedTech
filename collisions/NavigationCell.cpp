#include "NavigationCell.h"
#include "DXUtil.h"

int NavigationCell::climbable_incline = 10;
int NavigationCell::cell_size = 2;

NavigationCell::NavigationCell(int _x, float _y, int _z, XMFLOAT3 world)
{
	x = _x;
	y = _y;
	z = _z;
	world_pos = world;
}

void NavigationCell::checkNeighbour(NavigationCell* cell)
{
	for (NavigationCell* n : neighbours)
	{
		if (n == cell)
		{
			return;
		}
	}
	if (abs(cell->getIndexPos().x - x) <= 1 && abs(cell->getIndexPos().z - z) <= 1)
	{
		if (accessible && abs(cell->getIndexPos().y - y) <= climbable_incline)
		{
			if (cell != this)
			{
				neighbours[getFreeNeighbour()] = cell;
				cell->checkNeighbour(this);
			}
		}
	}
}

int NavigationCell::getFreeNeighbour()
{
	for (int i = 0; i < 8 ; i++)
	{
		if (neighbours[i] == nullptr)
			return i;
	}

	assert(false);
	return -1;
}
