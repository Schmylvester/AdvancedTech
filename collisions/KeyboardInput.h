#pragma once
#include <vector>

class KeyboardInput
{
public:
	KeyboardInput() = default;
	~KeyboardInput() = default;

	void keyDown(int key_id);
	void clearInputs();
	bool searchInputs(int key_id);

private:
	std::vector<int> inputs;
};

