#include "Application.h"
#include "BoxCollider.h"

Application::Application(HINSTANCE h_instance, Debug* _debug) : DXApp(h_instance, _debug)
{
	m_colour[0] = 0;
	m_colour[1] = 0;
	m_colour[2] = 0;
	m_colour[3] = 1;
}

Application::~Application()
{
	Memory::SafeDelete(m_fall_cube);
	Memory::SafeDelete(m_plane);
}

bool Application::init()
{
	if (!DXApp::init())
	{
		return false;
	}

	float colour[4]{ 1,1,1,1 };
	m_fall_cube = new Cube(this, colour);
	m_collision_detection.addPhysicsObject(m_fall_cube->getPhysics());

	m_plane = new Cube(this, colour);
	m_plane->move(0, -0.7f, 0);
	m_plane->getPhysics()->setGravity(0);
	m_collision_detection.addPhysicsObject(m_plane->getPhysics());
	return true;
}

void Application::update(float dt)
{
	if (input.searchInputs(32, KeyState::DOWN))
	{
		play = true;
	}
	input.tick();
	if (play)
	{
		m_cam.tick(dt);
		m_fall_cube->tick(dt);
		m_plane->tick(dt);
		m_collision_detection.checkCollisions();
	}
}

void Application::render(float dt)
{
	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	//m_dev->CreateRenderTargetView(pBackBuffer, NULL, &m_render_target_view);
	pBackBuffer->Release();

	//// set the render target as the back buffer
	m_dev_con->OMSetRenderTargets(1, &m_render_target_view, NULL);

	m_dev_con->ClearRenderTargetView(m_render_target_view, m_colour);

	m_fall_cube->draw();
	m_plane->draw();

	m_swap_chain->Present(0, 0);
}