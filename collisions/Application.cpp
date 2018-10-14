#include "Application.h"
#include "BoxCollider.h"

Application::Application(HINSTANCE h_instance) : DXApp(h_instance)
{
	m_colour[0] = 0;
	m_colour[1] = 0;
	m_colour[2] = 0;
	m_colour[3] = 1;
}

Application::~Application()
{
	for (Cube* cube : m_cubes)
	{
		Memory::SafeDelete(cube);
	}
}

void Application::initObjects()
{
	float colour[4] = { 1,1,1,1 };
	for (int x = -2; x < 2; x++)
		for (int y = -2; y < 2; y++)
			for (int z = -2; z < 2; z++)
			{
				m_cubes.push_back(new Cube(this, colour));
				m_cubes.back()->move(x * 0.25f, y * 0.25f, z * 0.25f);
				m_collision_detection.addPhysicsObject(m_cubes.back()->getPhysics());
				colour[rand() % 3] = (1 - (0.5f + (float)x / 4)) * (1 - (0.5f + (float)y / 4)) * (1 - (0.5f + (float)z / 4));
			}
}

bool Application::init()
{
	if (!DXApp::init())
	{
		return false;
	}

	initObjects();

	return true;
}

void Application::update(float dt)
{
	if (input.searchInputs(KeyBind::Space, KeyState::DOWN))
	{
		play = !play;
	}
	input.tick();
	m_cam->tick(dt);
	if (play)
	{
		for (Cube* cube : m_cubes)
		{
			cube->tick(dt);
		}
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
	m_dev_con->ClearDepthStencilView(m_depth_stncl_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);
	for (Cube* cube : m_cubes)
	{
		cube->draw();
	}

	m_swap_chain->Present(0, 0);
}