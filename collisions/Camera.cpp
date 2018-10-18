#include "Camera.h"
#include "DXApp.h"

Camera::Camera(Input * input, DXApp* _app)
{
	m_app = _app;
	m_input = input;
	using namespace DirectX;
	m_view_matrix = XMMatrixLookAtLH(m_transform.getPos(),
		XMVectorAdd(m_transform.getPos(), XMLoadFloat3(&m_target)), XMLoadFloat3(&m_up));
	m_perspective_matrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(70), _app->getRatio(), 0.1f, 100.0f);

	m_view_matrix = m_view_matrix * m_perspective_matrix;
}

Camera::~Camera()
{
}

void Camera::tick(float dt)
{
	XMFLOAT3 axis = XMFLOAT3(0, 1, 0);
	rotate(axis, dt * m_input->mouse.getMouseMove().x * 150);

	m_view_matrix = XMMatrixLookAtLH(m_transform.getPos(),
		XMLoadFloat3(&m_target), XMLoadFloat3(&m_up)) * m_perspective_matrix;
}

void Camera::rotate(XMFLOAT3 axis, float degrees)
{
	XMVECTOR temp_vec = XMLoadFloat3(&m_target) - XMLoadFloat3(&m_transform.getPos());
	XMStoreFloat3(&m_target, temp_vec);

	temp_vec = XMLoadFloat3(&m_up) - XMLoadFloat3(&m_transform.getPos());
	XMStoreFloat3(&m_up, temp_vec);

	temp_vec = XMVector3Transform(XMLoadFloat3(&m_target),
		XMMatrixRotationAxis(XMLoadFloat3(&axis), XMConvertToRadians(degrees)));
	XMStoreFloat3(&m_target, temp_vec);

	temp_vec = XMVector3Transform(XMLoadFloat3(&m_up),
		XMMatrixRotationAxis(XMLoadFloat3(&axis), XMConvertToRadians(degrees)));
	XMStoreFloat3(&m_up, temp_vec);
}