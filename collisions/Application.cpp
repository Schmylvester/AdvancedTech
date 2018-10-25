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
	for (Cube* cube : m_gameobjects)
	{
		Memory::SafeDelete(cube);
	}
}

void Application::initObjects()
{
	int i[3];
	for (i[0] = -20; i[0] < 20; i[0]++)
	{
		for (i[1] = -20; i[1] < 20; i[1]++)
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
	if (input.keyboard.searchInputs(KeyBind::Space, KeyState::DOWN))
	{
		play = !play;
	}
	m_cam->tick(dt);
	if (play)
	{
		for (Cube* cube : m_gameobjects)
		{
			cube->tick(dt);
		}
		m_collision_detection.checkCollisions();
	}
	input.keyboard.tick();
	input.mouse.tick();
}

void Application::render(float dt)
{
	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	pBackBuffer->Release();

	// set the render target as the back buffer
	m_dev_con->OMSetRenderTargets(1, &m_render_target_view, NULL);

	m_dev_con->ClearRenderTargetView(m_render_target_view, m_colour);
	m_dev_con->ClearDepthStencilView(m_depth_stncl_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);

	for (Cube* cube : m_gameobjects)
	{
		updateConstantBuffer(*(cube->getTransform()->getMatrix()), m_cam->getView());
		cube->draw();
	}

	m_swap_chain->Present(0, 0);
}