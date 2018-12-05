#include "NavigationCell.h"
#include "DXUtil.h"

bool NavigationCell::canGet(NavigationCell * from)
{
	if (!accessible)
	{
		return false;
	}
	return abs(from->getHeight() - y) <= 1;
}
