#include "Camera.h"
#include "DXApp.h"

Camera::Camera(KeyboardInput * input, DXApp* _app)
{
	m_app = _app;
	m_input = input;

	using namespace DirectX;
	m_transform.setMatrix(
		XMMatrixLookAtLH(m_transform.getPos(), XMVectorAdd(m_transform.getPos(), XMVectorSet(0, 0, 1, 1)), XMVectorSet(0, 1, 0, 1))
		* XMMatrixPerspectiveFovLH(XMConvertToRadians(75), _app->getRatio(), 0.1f, 100.0f));
}

Camera::~Camera()
{
}

void Camera::tick(float dt)
{
	if (m_input->searchInputs(KeyBind::W, KeyState::HELD))
		m_transform.move(0, dt, 0);
	if (m_input->searchInputs(KeyBind::S, KeyState::HELD))
		m_transform.move(0, -dt, 0);
	if (m_input->searchInputs(KeyBind::A, KeyState::HELD))
		m_transform.move(-dt, 0, 0);
	if (m_input->searchInputs(KeyBind::D, KeyState::HELD))
		m_transform.move(dt, 0, 0);
	if (m_input->searchInputs(KeyBind::U, KeyState::HELD))
		m_transform.rotate('x', dt);
	if (m_input->searchInputs(KeyBind::J, KeyState::HELD))
		m_transform.rotate('x', -dt);
	if (m_input->searchInputs(KeyBind::K, KeyState::HELD))
		m_transform.rotate('y', dt);
	if (m_input->searchInputs(KeyBind::H, KeyState::HELD))
		m_transform.rotate('y', -dt);

	m_transform.update();
}
