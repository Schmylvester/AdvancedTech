#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;
class MouseInput
{
public:
	MouseInput() = default;
	~MouseInput() = default;

	void move(int new_x, int new_y);
	void tick();
	Vector2 getMouseMove() { return mouse_pos - last_mouse_pos; }
	Vector2 getMousePos() { return mouse_pos; }
private:
	Vector2 mouse_pos = Vector2::Zero;
	Vector2 last_mouse_pos = Vector2::Zero;
};

