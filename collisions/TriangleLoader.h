#pragma once
#include "DXUtil.h"

class Camera;
class ID3D10Blob;
class TriangleLoader
{
public:
	TriangleLoader() = default;
	TriangleLoader(DXApp * app, Camera * _cam);
	~TriangleLoader();

	ID3D10Blob *VS, *PS;
private:
	Camera* m_cam;
};

