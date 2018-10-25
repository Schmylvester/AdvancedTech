#include "Camera.h"
#include "DXApp.h"

Camera::Camera(Input * input, DXApp* _app)
{
	m_app = _app;
	m_input = input;
	using namespace DirectX;
	m_view_matrix = XMMatrixLookAtLH(m_transform.getPos(),
		XMVectorAdd(m_transform.getPos(), XMLoadFloat3(&m_target)), XMLoadFloat3(&m_up));
	m_perspective_matrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(90), _app->getRatio(), 0.1f, 100.0f);

	m_view_matrix = m_view_matrix * m_perspective_matrix;
}

void Camera::tick(float dt)
{
	changeRotation();
	if (m_rotation_axis.x != 0
		|| m_rotation_axis.y != 0
		|| m_rotation_axis.z != 0)
		rotate(m_rotation_axis, dt * 15);

	m_view_matrix = XMMatrixLookAtLH(m_transform.getPos(),
		XMLoadFloat3(&m_target), XMLoadFloat3(&m_up)) * m_perspective_matrix;
}

void Camera::changeRotation()
{
	if (m_input->keyboard.searchInputs(KeyBind::Bar1, KeyState::DOWN))
		m_rotation_axis = XMFLOAT3(1, 0, 0);
	if (m_input->keyboard.searchInputs(KeyBind::Bar2, KeyState::DOWN))
		m_rotation_axis = XMFLOAT3(0, 1, 0);
	if (m_input->keyboard.searchInputs(KeyBind::Bar3, KeyState::DOWN))
		m_rotation_axis = XMFLOAT3(0, 0, 1);
	if (m_input->keyboard.searchInputs(KeyBind::Bar4, KeyState::DOWN))
		m_rotation_axis = XMFLOAT3(-1, 0, 0);
	if (m_input->keyboard.searchInputs(KeyBind::Bar5, KeyState::DOWN))
		m_rotation_axis = XMFLOAT3(0, -1, 0);
	if (m_input->keyboard.searchInputs(KeyBind::Bar6, KeyState::DOWN))
		m_rotation_axis = XMFLOAT3(0, 0, -1);
}

///this doesn't work
void Camera::move(float x, float y, float z)
{
	XMVECTOR temp_vec = XMLoadFloat3(&m_target) + XMLoadFloat3(&m_transform.getPos());
	XMStoreFloat3(&m_target, temp_vec);

	m_transform.move(x, y, z);
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