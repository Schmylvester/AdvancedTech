#include "TriangleLoader.h"
#include "DXUtil.h"
#include "Camera.h"

TriangleLoader::TriangleLoader(Camera* _cam)
{
	m_cam = _cam;
	D3DCompileFromFile(L"shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, &VS, 0);
	D3DCompileFromFile(L"shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, &PS, 0);
}

TriangleLoader::~TriangleLoader()
{
}