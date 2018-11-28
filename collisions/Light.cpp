#include "Light.h"

Light::Light()
{
	ZeroMemory(this, sizeof(Light));

	x = 0.9f;
	y = -0.7f;
	z = 0;
	dir = XMFLOAT3(x, y, z);
	ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}

void Light::update(float dt)
{
	//rotation += dt;
	//z = sin(rotation);
	//y = cos(rotation);

	//dir = XMFLOAT3(x, y, z);
}
