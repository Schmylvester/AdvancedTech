#include "Light.h"

Light::Light()
{
	ZeroMemory(this, sizeof(Light));

	x = 0;
	y = 0.2f;
	z = -1.0f;
	dir = XMFLOAT3(x, y, z);
	ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}

void Light::update(float dt)
{
	if (swing_dir == 0)
	{
		x -= dt * 0.1f;
		if (x < -0.5f)
		{
			swing_dir = 1;
		}
	}
	else
	{
		x += dt * 0.1f;
		if (x > 0.5f)
		{
			swing_dir = 0;
		}
	}
	dir = XMFLOAT3(x, y, z);
}
