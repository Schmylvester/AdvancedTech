#include "Transform.h"
class Camera
{
public:
	Camera();
	~Camera();

	Transform transform;
	void tick(float dt);
};