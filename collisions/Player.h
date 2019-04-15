#pragma once
#include "GameObject.h"

class Input;

class Player : public GameObject
{
public:
	Player() = default;
	Player(Input* _input);
	~Player() = default;

	virtual void update(float dt) override;
	virtual void collision(CollisionData col, CollisionClassifier type) override;

private:
	Input* m_input;
	float move_speed = 7;
};

