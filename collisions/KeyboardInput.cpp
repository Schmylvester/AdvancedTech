#include "KeyboardInput.h"

void KeyboardInput::keyDown(int key_id)
{
	inputs.push_back(key_id);
}

void KeyboardInput::clearInputs()
{
	inputs.clear();
}

bool KeyboardInput::searchInputs(int key_id)
{
	for (int i = 0; i < inputs.size(); i++)
	{
		if (inputs[i] == key_id)
			return true;
	}
	return false;
}
