#include "Light.h"

Light::Light()
{
	ZeroMemory(this, sizeof(Light));

	x = 0.9f;
	y = -0.7f;
	z = 0;
	dir = XMFLOAT3(x, y, z);
	ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
}

void Light::update(float dt)
{
	if (y > 0)
	{
		rotation += 0.3f * dt;
	}
	else
	{
		rotation += dt;
	}
	x = sin(rotation);
	y = cos(rotation);

	dir = XMFLOAT3(x, y, z);
}
