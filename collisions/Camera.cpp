#include "Camera.h"
#include "DXApp.h"

Camera::Camera(Input * input, DXApp* _app)
{
	m_app = _app;
	m_input = input;
	using namespace DirectX;
	m_view_matrix = XMMatrixLookAtLH(m_transform.getPos(),
		XMVectorAdd(m_transform.getPos(), XMVectorSet(0, 0, 1, 1)), XMVectorSet(0, 1, 0, 1));
	m_perspective_matrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(80), _app->getRatio(), 0.1f, 100.0f);
}

Camera::~Camera()
{
}

void Camera::tick(float dt)
{
	//m_view_matrix *= XMMatrixRotationY(m_input->mouse.getMouseMove().x * -dt);
	if (m_input->keyboard.searchInputs(KeyBind::A, KeyState::HELD))
	{
		m_view_matrix *= XMMatrixRotationY(dt);
	}
	if (m_input->keyboard.searchInputs(KeyBind::D, KeyState::HELD))
	{
		m_view_matrix *= XMMatrixRotationY(-dt);
	}
}