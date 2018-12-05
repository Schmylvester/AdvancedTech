#pragma once
class NavigationCell
{
public:
	NavigationCell() = default;
	~NavigationCell() = default;

	bool canGet(NavigationCell* from);
	int getHeight() { return y; }
private:
	bool accessible = true;
	int x, y, z;
};

