#pragma once
#include "DXUtil.h"

class Camera;
class ID3D10Blob;
class TriangleLoader
{
public:
	TriangleLoader() = default;
	TriangleLoader(Camera * _cam);
	~TriangleLoader();

	XMMATRIX getCamView();

	XMMATRIX getCamPerspective();

	ID3D10Blob *VS, *PS;
private:
	Camera* m_cam;
};

