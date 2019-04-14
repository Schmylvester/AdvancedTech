#include "Player.h"
#include "Input.h"
#include "Camera.h"
#include "PhysicsBody.h"

Player::Player(Input* _input)
{
	m_input = _input;
	rotation = 0;
}

void Player::update(float dt)
{
	float step = dt * move_speed;

	if (m_input->key(DIK_W, InputState::HELD))
	{
		m_transform.translate(0, 0, step);
	}
	if (m_input->key(DIK_A, InputState::HELD))
	{
		m_transform.translate(-step, 0, 0);
	}
	if (m_input->key(DIK_S, InputState::HELD))
	{
		m_transform.translate(0, 0, -step);
	}
	if (m_input->key(DIK_D, InputState::HELD))
	{
		m_transform.translate(step, 0, 0);
	}
	if (m_input->key(DIK_SPACE, InputState::DOWN))
	{
		m_physics->addForceAtPoint(9, Vector3(0, -1, 0), Vector3(0, 1, 0));
	}

	m_cam->lookAt(m_transform.getPos());
	GameObject::update(dt);
}

void Player::collision(CollisionData col, CollisionClassifier type)
{
	//if (type == CollisionClassifier::Collision_This_Frame)
		////m_physics->addForceAtPoint(0.3f, col.colision_center, m_transform.getPos() - col.colision_center);
}
