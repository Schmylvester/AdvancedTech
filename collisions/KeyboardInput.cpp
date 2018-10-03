#include "KeyboardInput.h"

void KeyboardInput::keyDown(int key_id)
{
	key_down.push_back(key_id);
	key.push_back(key_id);
}
void KeyboardInput::keyUp(int key_id)
{
	key_up.push_back(key_id);
	for (int i = 0; i < key.size(); i++)
	{
		if (key[i] == key_id)
		{
			key.erase(key.begin() + i);
		}
	}
}

bool KeyboardInput::searchInputs(int key_id, KeyState type)
{
	std::vector<int>* check = nullptr;
	switch (type)
	{
	case KeyState::DOWN:
		check = &key_down;
		break;
	case KeyState::HELD:
		check = &key;
		break;
	case KeyState::UP:
		check = &key_up;
		break;
	default:
		break;
	}
	for (int i = 0; i < (*check).size(); i++)
	{
		if ((*check)[i] == key_id)
			return true;
	}
	return false;
}

void KeyboardInput::tick()
{
	key_down.clear();
	key_up.clear();
}
