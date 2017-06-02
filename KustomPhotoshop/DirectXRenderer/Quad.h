#pragma once

#include <vector>
#include "DirectXDependencies.h"
#include "DirectXGraphics.h"
#include "PolyLine.h"
#include "Camera2D.h"

using namespace std;


struct QuadCB
{
	XMFLOAT4X4 viewProj;
};


class Quad
{
	private:

	ID3D11InputLayout *inputLayout;
	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer *constantBuffer;
	ID3D11RasterizerState *basicRasterizerState;
	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *pixelShader;

	PolyLine borderLine;

	public:

	Quad();
	~Quad();

	HRESULT InitFX(ID3D11Device *device, LPCWSTR fileName);
	HRESULT InitGeometry(ID3D11Device *device, XMFLOAT3 v1, XMFLOAT3 v2, XMFLOAT3 v3, XMFLOAT3 v4, XMFLOAT4 color, XMFLOAT4 borderColor, float width_);

	void Render(ID3D11DeviceContext *painter, Camera2D &camera);

	void ReleaseFX();
	void ReleaseGeometry();
	void Release();
};

