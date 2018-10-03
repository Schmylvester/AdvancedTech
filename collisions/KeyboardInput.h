#pragma once
#include <vector>

enum class KeyState
{
	DOWN,
	HELD,
	UP,
};

class KeyboardInput
{
public:
	KeyboardInput() = default;
	~KeyboardInput() = default;

	void keyDown(int key_id);
	void keyUp(int key_id);
	bool searchInputs(int key_id, KeyState type);
	void tick();

private:
	std::vector<int> key_down;
	std::vector<int> key;
	std::vector<int> key_up;
};

