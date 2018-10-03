#pragma once
#include "DXApp.h"

class Cube;
class Application : public DXApp
{
public:
	Application(HINSTANCE h_instance, Debug* _debug);
	~Application();

	bool init() override;
	void update(float dt) override;
	void render(float dt) override;
private:
	Cube* m_cube;
};

