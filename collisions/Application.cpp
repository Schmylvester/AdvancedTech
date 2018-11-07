#include "Application.h"
#include "BoxCollider.h"


Application::~Application()
{
	releaseObjects();
	for (Cube* cube : m_gameobjects)
	{
		Memory::SafeDelete(cube);
	}
}

void Application::initObjects()
{
	int i[3];
	for (i[0] = -6; i[0] < 6; i[0]++)
	{
		for (i[1] = -6; i[1] < 6; i[1]++)
		{
			for (i[2] = 0; i[2] < 1; i[2]++)
			{
				float x_pos = i[0] * 1.5f;
				float y_pos = i[1] * 1.5f;
				float z_pos = 10;

				float colour[4] = { 1, 1, 1, 1 };
				colour[rand() % 3] = 0.3f;
				colour[rand() % 3] = 0.3f;
				m_gameobjects.push_back(new Cube(this, colour));
				m_gameobjects.back()->move(x_pos, y_pos, z_pos);
				m_collision_detection.addPhysicsObject(m_gameobjects.back()->getPhysics());
			}
		}
	}
	return;
}

bool Application::init(HINSTANCE h_instance)
{
	if (!DXApp::init(h_instance))
	{
		return false;
	}

	//initObjects();

	return true;
}