// DirectXRenderer.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <vector>
#include <map>
#include "DirectXGraphics.h"
#include "PolyLine.h"
#include "Triangle.h"
#include "BoundingBox.h"
#include "Camera2D.h"


using namespace std;


#define CPP_EXPORTS_API
#ifdef CPP_EXPORTS_API
#define CPP_API extern "C" __declspec(dllexport)
#else
#define CPP_API __declspec(dllimport)
#endif


DirectXGraphics graphics;
Camera2D camera;

int nextPolyLineIndex;
int nextTriangleIndex;

map<int, PolyLine> polyLines;
map<int, Triangle> triangles;

BoundingBox boundingBox;


CPP_API void Init(int hwnd)
{
	graphics.Init((HWND)hwnd);

	boundingBox.InitFX(graphics.GetDevice(), L"BoundingBoxShader.hlsl");

	camera.SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));

	nextPolyLineIndex = 0;
	nextTriangleIndex = 0;
}


CPP_API void Resize(int hwnd)
{
	graphics.Resize((HWND)hwnd);

	RECT rc;
	GetClientRect((HWND)hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	camera.SetOrthographicView(height, (float)width / height);
}


CPP_API void Release()
{
	for (auto it = polyLines.begin(); it != polyLines.end(); it++)
	{
		it->second.Release();
	}

	for (auto it = triangles.begin(); it != triangles.end(); it++)
	{
		it->second.Release();
	}

	boundingBox.Release();

	graphics.Release();
}


CPP_API void BeginRender()
{
	graphics.BeginRender();
}


CPP_API void FinishRender()
{
	graphics.FinishRender();
}


CPP_API void SetCameraPosition(float x, float y)
{
	camera.SetPosition(XMFLOAT3(x, y, 0.0f));
}


CPP_API void SetCameraZoom(float scale)
{
	camera.SetZoom(scale);
}


CPP_API void SetBoundingBoxZoom(float scale)
{
	boundingBox.SetZoom(scale);
}


CPP_API int AddPolyLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float x5, float y5, float x6, float y6, int pointsNum, float r, float g, float b, float width)
{
	PolyLine polyLine;
	polyLine.InitFX(graphics.GetDevice(), L"PolyLineShader.hlsl");

	vector<float> points { x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6 };
	points.resize(pointsNum * 2);

	polyLine.InitGeometry(graphics.GetDevice(), points, width, XMFLOAT4(r, g, b, 1.0f), 0.0f);
	polyLines[nextPolyLineIndex] = polyLine;
	nextPolyLineIndex++;

	return nextPolyLineIndex - 1;
}


CPP_API int AddTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b, float rB, float gB, float bB, float width)
{
	Triangle triangle;
	triangle.InitFX(graphics.GetDevice(), L"TriangleShader.hlsl");
	triangle.InitGeometry(graphics.GetDevice(), XMFLOAT3(x1, y1, 0.0f), XMFLOAT3(x2, y2, 0.0f), XMFLOAT3(x3, y3, 0.0f), XMFLOAT4(r, g, b, 1.0f), XMFLOAT4(rB, gB, bB, 1.0f), width);
	triangles[nextTriangleIndex] = triangle;
	nextTriangleIndex++;

	return nextTriangleIndex - 1;
}


CPP_API void ModifyPolyLine(int polyLineIndex, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float x5, float y5, float x6, float y6, int pointsNum, float r, float g, float b, float width)
{
	polyLines[polyLineIndex].ReleaseGeometry();

	vector<float> points {x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6};
	points.resize(pointsNum * 2);

	polyLines[polyLineIndex].InitGeometry(graphics.GetDevice(), points, width, XMFLOAT4(r, g, b, 1.0f), 0.0f);
}


CPP_API void ModifyTriangle(int triangleIndex, float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b, float rB, float gB, float bB, float width)
{
	triangles[triangleIndex].ReleaseGeometry();
	triangles[triangleIndex].InitGeometry(graphics.GetDevice(), XMFLOAT3(x1, y1, 0.0f), XMFLOAT3(x2, y2, 0.0f), XMFLOAT3(x3, y3, 0.0f), XMFLOAT4(r, g, b, 1.0f), XMFLOAT4(rB, gB, bB, 1.0f), width);
}


CPP_API void ModifyBoundingBox(float xMin, float yMin, float xMax, float yMax)
{
	boundingBox.ReleaseGeometry();
	boundingBox.InitGeometry(graphics.GetDevice(), XMFLOAT3(xMin, yMin, 0.0f), XMFLOAT3(xMax, yMax, 0.0f));
}


CPP_API void RenderPolyLine(int polyLineIndex)
{
	polyLines[polyLineIndex].Render(graphics.GetPainter(), camera);
}


CPP_API void RenderTriangle(int triangleIndex)
{
	triangles[triangleIndex].Render(graphics.GetPainter(), camera);
}


CPP_API void RenderBoundingBox()
{
	boundingBox.Render(graphics.GetPainter(), camera);
}


CPP_API void RemovePolyLine(int polyLineIndex)
{
	polyLines[polyLineIndex].Release();
	polyLines.erase(polyLineIndex);
}


CPP_API void RemoveTriangle(int triangleIndex)
{
	triangles[triangleIndex].Release();
	triangles.erase(triangleIndex);
}