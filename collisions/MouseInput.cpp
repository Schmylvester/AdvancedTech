#include "MouseInput.h"

void MouseInput::move(int new_x, int new_y)
{
	if (last_mouse_pos != Vector2::Zero)
	{
		last_mouse_pos = mouse_pos;
	}
	else
	{
		last_mouse_pos = Vector2(new_x, new_y);
	}
	mouse_pos = Vector2(new_x, new_y);
}

void MouseInput::tick()
{
	last_mouse_pos = mouse_pos;
}
