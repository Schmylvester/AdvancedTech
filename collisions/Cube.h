#pragma once
#include "Geometry.h"
#include "Transform.h"

using namespace DirectX::SimpleMath;

class DXApp;
class RenderCube;
class PhysicsCube;

class Cube
{
public:
	Cube(DXApp * app, float colour[4], Transform transform);
	~Cube();

	void draw();
	void tick(float dt);

	PhysicsCube* getPhysics();
private:
	Transform m_transform;

	Vector3 m_corners[8];
	RenderCube* m_render;
	PhysicsCube* m_physics;
	void setRenderTriangles();
};

