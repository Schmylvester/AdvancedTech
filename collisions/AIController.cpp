#include "AIController.h"
#include "Cube.h"
#include "Pathfinder.h"
#include "NavigationCell.h"
#include "DXApp.h"
#include "ConstantBuffers.h"
#include "Camera.h"

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
	renderer->getTransform()->scale(6, 90, 6);
	target_renderer->getTransform()->scale(6, 90, 6);
}

AIController::~AIController()
{
	Memory::SafeDelete(renderer);
}

void AIController::assignPath(NavigationCell * to)
{
	if (!on_path)
	{
		on_path = true;
		path_index = 0;
		Pathfinder find;
		path = find.findpath(position, to);

		Transform* t = target_renderer->getTransform();
		t->setPosition(to->getWorldPos().x, to->getWorldPos().y / 3, to->getWorldPos().z);
	}
}

void AIController::update(float dt)
{
	if (path.size() == 0)
	{
		path_index = 0;
		on_path = false;
	}
	if (on_path)
	{
		timer += dt;
		if (timer >= 0.07)
		{
			timer -= 0.07;
			position = path[++path_index];
			if (path_index == path.size() - 1)
			{
				path_index = 0;
				on_path = false;
			}
			setRendererPos();
		}
	}
}

void AIController::draw()
{
	renderer->draw();
	if (on_path)
	{
		target_renderer->draw();
	}
}

void AIController::setRendererPos()
{
	Transform* t = renderer->getTransform();
	t->setPosition(position->getWorldPos().x, position->getWorldPos().y / 3, position->getWorldPos().z);
}