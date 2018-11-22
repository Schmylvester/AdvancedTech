#pragma once
#include "DXApp.h"

class Scene : public DXApp
{
public:
	Scene() = default;
	~Scene() = default;

	virtual void updateScene(float dt) override;
	virtual void drawScene(float dt) override;
	virtual void initObjects() override;
};