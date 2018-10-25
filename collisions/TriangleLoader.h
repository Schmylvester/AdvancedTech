#pragma once
#include "DXUtil.h"

class Camera;
class ID3D10Blob;
class DXApp;
class TriangleLoader
{
public:
	TriangleLoader() = default;
	TriangleLoader(DXApp * app, Camera * _cam);
	~TriangleLoader();

	ID3D11VertexShader* m_vtx_shader;
	ID3D11PixelShader* m_pxl_shader;
	ID3D10Blob *VS, *PS;
private:
	Camera* m_cam;

};

