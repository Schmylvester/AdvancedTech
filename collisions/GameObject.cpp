#include "GameObject.h"
#include "GeometryIncludes.h"
#include "DXUtil.h"
#include "DXApp.h"
#include "CollisionManager.h"

GameObject::~GameObject()
{
	Memory::SafeDelete(m_geometry);
	Memory::SafeDelete(m_collider);
}

void GameObject::init(Shape shape, DXApp * _app, CBPerObject * _cb, Camera * cam, ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff)
{
	m_cam = cam;
	switch (shape)
	{
	case Shape::Cube:
		m_geometry = new Cube();
		break;
	case Shape::Pyramid:
		m_geometry = new Pyramid();
		break;
	case Shape::Plane:
		m_geometry = new PlaneObject();
		break;
	case Shape::Frustum:
		m_geometry = new Frustum();
		break;
	default:
		break;
	}
	m_geometry->init(_app, _cb, cam, dev_con, c_buff, this);
}

void GameObject::init(const char* file, int x, int y, DXApp * _app,
	CBPerObject * _cb, Camera * cam, 
	ID3D11DeviceContext * dev_con, ID3D11Buffer * c_buff)
{
	m_geometry = new Terrain(file, x, y);
	m_geometry->init(_app, _cb, cam, dev_con, c_buff, this);
}

void GameObject::addCollider(Collider * col, CollisionManager * collision_manager, bool add_physics)
{
	collision_manager->addCollider(col);
	m_collider = col;
	if (add_physics)
	{
		m_physics = std::make_unique<PhysicsBody>(m_collider, &m_transform);
	}
}

void GameObject::collision(CollisionData col, CollisionClassifier type)
{
	if (type == CollisionClassifier::Collision_This_Frame)
	{
		rotation *= -1;
	}
}

Transform * GameObject::getTransform()
{
	return &m_transform;
}

Geometry * GameObject::getGeometry()
{
	return m_geometry;
}

void GameObject::update(float dt)
{
	m_transform.rotate(rotDir, rotation * dt);

	if (m_physics != nullptr)
	{
		m_physics->tick(dt);
	}
}

void GameObject::draw()
{
	m_geometry->draw(m_transform.getWorldMatrix());
}
