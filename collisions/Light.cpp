#include "Light.h"

Light::Light()
{
	ZeroMemory(this, sizeof(Light));

	x = 0.8f;
	y = 0.6f;
	z = 0;
	dir = XMFLOAT3(x, y, z);
	ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
}
