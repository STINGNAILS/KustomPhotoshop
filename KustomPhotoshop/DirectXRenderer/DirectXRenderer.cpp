// DirectXRenderer.cpp : Defines the exported functions for the DLL application.
//

#include <vector>
#include "stdafx.h"
#include "DirectXGraphics.h"
#include "Triangle.h"
#include "Camera2D.h"


using namespace std;


#define CPP_EXPORTS_API
#ifdef CPP_EXPORTS_API
#define CPP_API extern "C" __declspec(dllexport)
#else
#define CPP_API __declspec(dllimport)
#endif


extern DirectXGraphics graphics;
extern Camera2D camera;

vector<Triangle> triangles;


CPP_API void Init(int hwnd)
{
	graphics.Init((HWND)hwnd);

	RECT rc;
	GetClientRect((HWND)hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	camera.SetOrthographicView(height, (float)width / height);
	camera.SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
}


CPP_API int AddTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b, float rB, float gB, float bB, float width)
{
	Triangle triangle;
	triangle.InitFX(graphics.GetDevice(), L"Triangle.hlsl");
	triangle.InitGeometry(graphics.GetDevice(), XMFLOAT3(x1, y1, 0.0f), XMFLOAT3(x2, y2, 0.0f), XMFLOAT3(x3, y3, 0.0f), XMFLOAT4(r, g, b, 1.0f), XMFLOAT4(rB, gB, bB, 1.0f), width);

	triangles.push_back(triangle);

	return triangles.size() - 1;
}


CPP_API int ModifyTriangle(int triangleIndex, float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b, float rB, float gB, float bB, float width)
{
	triangles[triangleIndex].ReleaseGeometry();
	triangles[triangleIndex].InitGeometry(graphics.GetDevice(), XMFLOAT3(x1, y1, 0.0f), XMFLOAT3(x2, y2, 0.0f), XMFLOAT3(x3, y3, 0.0f), XMFLOAT4(r, g, b, 1.0f), XMFLOAT4(rB, gB, bB, 1.0f), width);
}