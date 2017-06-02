#pragma once

#include <vector>
#include "DirectXDependencies.h"
#include "DirectXGraphics.h"
#include "Camera2D.h"

using namespace std;


struct EllipseCB
{
	XMFLOAT4X4 viewProj;
	XMFLOAT4 abWidth;
};


class Ellipse0
{
	private:

	float a;
	float b;
	float width;

	ID3D11InputLayout *inputLayout;
	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer *constantBuffer;
	ID3D11RasterizerState *basicRasterizerState;
	ID3D11BlendState *transparentBlendState;
	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *pixelShader;

	public:

	Ellipse0();
	~Ellipse0();

	HRESULT InitFX(ID3D11Device *device, LPCWSTR fileName);
	HRESULT InitGeometry(ID3D11Device *device, XMFLOAT3 o, float a, float b, float angle, XMFLOAT4 color, XMFLOAT4 borderColor, float width_);

	void Render(ID3D11DeviceContext *painter, Camera2D &camera);

	void ReleaseFX();
	void ReleaseGeometry();
	void Release();
};

