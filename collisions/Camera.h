#include "Transform.h"
#include "KeyboardInput.h"
class Camera
{
public:
	Camera() = delete;
	Camera(KeyboardInput* input);
	~Camera();

	Transform transform;
	void tick(float dt);

private:
	KeyboardInput * m_input;
};