#include "GameObject.h"
#include "GeometryIncludes.h"
#include "DXUtil.h"
#include "DXApp.h"

GameObject::~GameObject()
{
	Memory::SafeDelete(m_geometry);
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

}

void GameObject::draw()
{
	m_geometry->draw(m_transform.getWorldMatrix());
}
