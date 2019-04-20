#pragma once
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include <D3Dcompiler.h>
#pragma comment(lib, "D3DCompiler.lib")
#include <SimpleMath.h>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
namespace Memory
{
	template <class T> void SafeDelete(T& t)
	{
		if (t)
		{
			delete t;
			t = nullptr;
		}
	}

	template <class T> void SafeDeleteArr(T& t)
	{
		if (t)
		{
			delete[] t;
			t = nullptr;
		}
	}

	template <class T> void SafeRelease(T& t)
	{
		if (t)
		{
			t->Release();
			t = nullptr;
		}
	}
}
