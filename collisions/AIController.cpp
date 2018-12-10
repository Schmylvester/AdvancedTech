#include "AIController.h"
#include "Cube.h"
#include "Pathfinder.h"
#include "NavigationCell.h"
#include "DXApp.h"
#include "ConstantBuffers.h"
#include "Camera.h"

bool g_thread_ready = true;
std::vector<NavigationCell*>* g_path;
NavigationCell * g_from;
NavigationCell * g_to;
AIController* g_ai;
void threadedPathFinder()
{
	g_thread_ready = false;
	Pathfinder path_finder;
	*(g_path) = path_finder.findpath(g_from, g_to);
	g_ai->setState(AIState::Moving);
	g_thread_ready = true;
}

AIController::AIController(DXApp * _app, CBPerObject * _cb, Camera * cam,
	ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff, NavigationCell* _cell)
{
	int colour = rand() % 6;
	renderer = new Cube();
	renderer->setColour(colour);
	renderer->init(_app, _cb, cam, dev_con, c_buff);
	target_renderer = new Cube();
	target_renderer->setColour(colour);
	target_renderer->init(_app, _cb, cam, dev_con, c_buff);
	position = _cell;
	renderer->getTransform()->scale(2, 150, 2);
	target_renderer->getTransform()->scale(2, 90, 2);
}

AIController::~AIController()
{
	Memory::SafeDelete(renderer);
}

void AIController::assignPath(NavigationCell * to)
{
	if (state == AIState::Idle && g_thread_ready)
	{
		path_index = 0;

		g_path = &path;
		g_from = position;
		g_to = to;
		g_ai = this;
		path_thread = std::thread(threadedPathFinder);

		Transform* t = target_renderer->getTransform();
		t->setPosition(to->getWorldPos().x, to->getWorldPos().y / 3, to->getWorldPos().z);
	}
}

void AIController::update(float dt)
{
	if (g_thread_ready && path_thread.joinable())
	{
		path_thread.join();
	}
	if (path.size() <= 1)
	{
		path_index = 0;
		state = AIState::Idle;
	}
	if (state == AIState::Moving)
	{
		timer += dt;
		if (timer >= 0.1)
		{
			timer -= 0.1;
			position = path[++path_index];
			if (path_index == path.size() - 1)
			{
				path_index = 0;
				state = AIState::Idle;
			}
			setRendererPos();
		}
	}
}

void AIController::draw()
{
	renderer->draw();
	if (state == AIState::Moving)
	{
		target_renderer->draw();
	}
}

void AIController::setRendererPos()
{
	Transform* t = renderer->getTransform();
	t->setPosition(position->getWorldPos().x, position->getWorldPos().y / 3, position->getWorldPos().z);
}