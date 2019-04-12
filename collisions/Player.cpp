#include "Player.h"
#include "Input.h"
#include "Camera.h"

Player::Player(Input* _input)
{
	m_input = _input;
}

void Player::update(float dt)
{
	float step = dt * move_speed;

	if (m_input->key(DIK_W, InputState::HELD))
	{
		m_transform.translate(0, 0, step);
		m_cam->move(0, 0, step);
	}
	if (m_input->key(DIK_A, InputState::HELD))
	{
		m_transform.translate(-step, 0, 0);
		m_cam->move(-step, 0, 0);
	}
	if (m_input->key(DIK_S, InputState::HELD))
	{
		m_transform.translate(0, 0, -step);
		m_cam->move(0, 0, -step);
	}
	if (m_input->key(DIK_D, InputState::HELD))
	{
		m_transform.translate(step, 0, 0);
		m_cam->move(step, 0, 0);
	}
}
