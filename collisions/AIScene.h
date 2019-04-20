#pragma once
#include "DXApp.h"

class Terrain;
class Player;
class AI;
class AIScene : public DXApp
{
public:
	AIScene() = default;
	~AIScene();

	virtual void updateScene(float dt) override;
	virtual void drawScene(float dt) override;
	virtual void initObjects() override;

private:
	Terrain * map;
	Player * player;
	AI* ai;
	GameObject* ai_target;
};