#include "Player.h"
#include "Input.h"
#include "Camera.h"
#include "PhysicsBody.h"

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
		m_collider->objectMoved();
	}
	if (m_input->key(DIK_A, InputState::HELD))
	{
		m_transform.translate(-step, 0, 0);
		m_cam->move(-step, 0, 0);
		m_collider->objectMoved();
	}
	if (m_input->key(DIK_S, InputState::HELD))
	{
		m_transform.translate(0, 0, -step);
		m_cam->move(0, 0, -step);
		m_collider->objectMoved();
	}
	if (m_input->key(DIK_D, InputState::HELD))
	{
		m_transform.translate(step, 0, 0);
		m_cam->move(step, 0, 0);
		m_collider->objectMoved();
	}
	if (m_input->key(DIK_E, InputState::HELD))
	{
		m_transform.translate(0, step, 0);
		m_cam->move(0, step, 0);
		m_collider->objectMoved();
	}
	if (m_input->key(DIK_Q, InputState::HELD))
	{
		m_transform.translate(0, -step, 0);
		m_cam->move(0, -step, 0);
		m_collider->objectMoved();
	}

	if (m_input->mouseMove().x > 0)
	{
		m_cam->rotateAround(5);
	}

	m_cam->setPos(m_transform.getPos().x, m_transform.getPos().y + 3.5f, m_transform.getPos().z - 8);

	GameObject::update(dt);
}

void Player::collision(CollisionData col, CollisionClassifier type)
{
	GameObject::collision(col, type);
}
