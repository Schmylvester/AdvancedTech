#pragma once
#include "Geometry.h"
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

class DXApp;
class RenderCube;
class PhysicsCube;

class Cube
{
public:
	Cube(DXApp* app);
	~Cube();

	void draw();
	void tick(float dt);
private:
	Vector3 m_position;
	Vector3 m_rotation;
	Vector3 m_scale;

	Vector3 m_corners[8];
	RenderCube* m_render;
	PhysicsCube* m_physics;
	void setRenderTriangles();
};

