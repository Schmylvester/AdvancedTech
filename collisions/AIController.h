#pragma once
#include <vector>
#include <thread>
class Cube;
class NavigationCell;
class DXApp;
class CBPerObject;
class Camera;
class ID3D11DeviceContext;
class ID3D11Buffer;

enum class AIState
{
	Idle,
	Moving
};

class AIController
{
public:
	AIController() = default;
	AIController(DXApp* _app, CBPerObject * _cb, Camera * cam,
		ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff, NavigationCell* _cell);
	~AIController();

	void assignPath(NavigationCell* to);
	void update(float dt);
	void draw();
	void setState(AIState _state) { state = _state; }
private:
	void setRendererPos();
	float timer = 0;
	int path_index;
	Cube* renderer = nullptr;
	Cube* target_renderer = nullptr;
	NavigationCell* position = nullptr;
	std::vector<NavigationCell*> path;
	AIState state = AIState::Idle;
	std::thread path_thread;
};

