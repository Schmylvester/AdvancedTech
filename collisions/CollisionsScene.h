#pragma once
#include "Scene.h"
#include "Terrain.h"
#include "BoxCollider.h"

class CollisionsScene : public Scene
{
public:
	CollisionsScene() = default;
	~CollisionsScene();

	virtual void updateScene(float dt) override;
	virtual void drawScene(float dt) override;
	virtual void initObjects() override;
private:
	std::unique_ptr<Geometry> player = nullptr;
	std::unique_ptr <BoxCollider> playerCol = nullptr;
	std::unique_ptr <Geometry> player2 = nullptr;
	std::unique_ptr <BoxCollider> player2Col = nullptr;
	
	float move_speed = 7;
	bool rot_dir = true;
};