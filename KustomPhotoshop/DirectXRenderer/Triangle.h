#pragma once

#include <vector>
#include "DirectXDependencies.h"
#include "DirectXGraphics.h"
#include "Camera2D.h"

using namespace std;


struct TriangleCB
{
	XMFLOAT4X4 viewProj;
	XMFLOAT4 v1v2;
	XMFLOAT4 v3Width;
};


class Triangle
{
	private:

	float width;

	XMFLOAT3 v1;
	XMFLOAT3 v2;
	XMFLOAT3 v3;

	ID3D11InputLayout *inputLayout;
	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer *triangleConstantBuffer;
	ID3D11RasterizerState *basicRasterizerState;
	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *pixelShader;

	public:

	Triangle();
	~Triangle();

	HRESULT InitFX(ID3D11Device *device, LPCWSTR fileName);
	HRESULT InitGeometry(ID3D11Device *device, XMFLOAT3 v1_, XMFLOAT3 v2_, XMFLOAT3 v3_, XMFLOAT4 color, XMFLOAT4 borderColor, float width_);

	void Render(ID3D11DeviceContext *painter, Camera2D &camera);

	void ReleaseFX();
	void ReleaseGeometry();
	void Release();
};

