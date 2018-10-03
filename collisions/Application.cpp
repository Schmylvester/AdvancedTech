#include "Application.h"
#include "Cube.h"

Application::Application(HINSTANCE h_instance, Debug* _debug) : DXApp(h_instance, _debug)
{
	m_colour[0] = 0;
	m_colour[1] = 0;
	m_colour[2] = 0;
	m_colour[3] = 1;
}

Application::~Application()
{
	Memory::SafeDelete(m_cube);
}

bool Application::init()
{
	if (!DXApp::init())
	{
		return false;
	}

	Transform cube_transform;
	cube_transform.position[0] = 0;
	cube_transform.position[1] = 0;
	cube_transform.position[2] = 0;
	cube_transform.scale[0] = 1;
	cube_transform.scale[1] = 1;
	cube_transform.scale[2] = 1;
	m_cube = new Cube(this, cube_transform);

	return true;
}

void Application::update(float dt)
{
	input.clearInputs();
}

void Application::render(float dt)
{
	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	m_dev->CreateRenderTargetView(pBackBuffer, NULL, &m_render_target_view);
	pBackBuffer->Release();

	// set the render target as the back buffer
	m_dev_con->OMSetRenderTargets(1, &m_render_target_view, NULL);

	m_dev_con->ClearRenderTargetView(m_render_target_view, m_colour);

	m_cube->draw();

	m_swap_chain->Present(0, 0);
}