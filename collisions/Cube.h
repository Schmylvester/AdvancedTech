#pragma once
#include "Geometry.h"
#include "Transform.h"

using namespace DirectX::SimpleMath;

class DXApp;
class RenderCube;
class Collider;

class Cube
{
public:
	Cube(DXApp * app, float colour[4]);
	~Cube();

	void draw();
	void tick(float dt);

	void move(float x, float y, float z);
	void rotate(char axis, float rot);

	void setScl(float x, float y, float z);

	Collider* getPhysics();
	Transform* getTransform() { return m_transform; }
private:
	Transform* m_transform;

	Vector3 m_corners[8];
	RenderCube* m_render;
	Collider* m_physics;
	void setRenderTriangles(bool init);
};

