#include "NavigationCell.h"
#include "DXUtil.h"

int NavigationCell::climbable_incline = 1;

void NavigationCell::checkNeighbour(NavigationCell* cell)
{
	if (abs(cell->getPos().x - x) <= 1 && abs(cell->getPos().z - z) <= 1)
	{
		if (accessible && abs(cell->getPos().y - y) <= climbable_incline)
		{
			neighbours.push_back(cell);
		}
	}
}