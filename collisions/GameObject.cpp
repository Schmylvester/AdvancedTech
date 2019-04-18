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
	case Shape::Sphere:
		m_geometry = new Sphere();
		break;
	default:
		break;
	}
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
	if (type == CollisionClassifier::Ongoing_Collision)
	{
		//move out of the object
		m_transform.translate(col.collision_direction * col.penetration / 2);
		m_collider->objectMoved();
	}
	if (type == CollisionClassifier::Collision_This_Frame)
	{
		if (m_physics != nullptr)
		{
			//m_physics->addForceAtPoint(1.0f, m_transform.getPos() - col.other_object->getTransform()->getPos(), col.collision_direction);
		}
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
	if (m_physics != nullptr)
	{
		m_physics->tick(dt);
	}
}

void GameObject::draw()
{
	m_geometry->draw(m_transform.getWorldMatrix());
}
