#include "TriangleLoader.h"
#include "Camera.h"
#include "DXApp.h"

TriangleLoader::TriangleLoader(DXApp* app, Camera* _cam)
{
	m_cam = _cam;
	D3DCompileFromFile(L"shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, &VS, 0);
	D3DCompileFromFile(L"shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, &PS, 0);
	
	auto device = app->getDevice();
	device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_vtx_shader);
	device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pxl_shader);

	auto context = app->getContext();
	context->VSSetShader(m_vtx_shader, 0, 0);
	context->PSSetShader(m_pxl_shader, 0, 0);
}

TriangleLoader::~TriangleLoader()
{
	Memory::SafeRelease(m_pxl_shader);
	Memory::SafeRelease(m_vtx_shader);
}
