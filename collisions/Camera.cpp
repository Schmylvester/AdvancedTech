#include "Camera.h"

Camera::Camera(KeyboardInput * input)
{
	m_input = input;
}

Camera::~Camera()
{
}

void Camera::tick(float dt)
{
	if (m_input->searchInputs(KeyBind::W, KeyState::HELD))
		transform.move(0, dt, 0);
	if (m_input->searchInputs(KeyBind::S, KeyState::HELD))
		transform.move(0, -dt, 0);
	if (m_input->searchInputs(KeyBind::A, KeyState::HELD))
		transform.move(-dt, 0, 0);
	if (m_input->searchInputs(KeyBind::D, KeyState::HELD))
		transform.move(dt, 0, 0);
	if (m_input->searchInputs(KeyBind::U, KeyState::HELD))
		transform.rotate('x', dt);
	if (m_input->searchInputs(KeyBind::J, KeyState::HELD))
		transform.rotate('x', -dt);
	if (m_input->searchInputs(KeyBind::K, KeyState::HELD))
		transform.rotate('y', dt);
	if (m_input->searchInputs(KeyBind::H, KeyState::HELD))
		transform.rotate('y', -dt);

	transform.update();
}
