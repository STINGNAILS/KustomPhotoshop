#pragma once

#include "stdafx.h"
#include <vector>
#include "DirectXGraphics.h"
#include "Camera2D.h"

using namespace std;


struct PolyLineCB
{
	XMFLOAT4X4 viewProj;
};


class PolyLine
{
	private:

	int lineVertexNum;
	int pointVertexNum;

	ID3D11InputLayout *inputLayout;
	ID3D11Buffer *vertexBufferLines;
	ID3D11Buffer *vertexBufferPoints;
	ID3D11Buffer *constantBuffer;
	ID3D11RasterizerState *basicRasterizerState;
	ID3D11BlendState *transparentBlendState;
	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *pixelShaderLines;
	ID3D11PixelShader *pixelShaderPoints;

	public:

	PolyLine();
	~PolyLine();

	HRESULT InitFX(ID3D11Device *device, LPCWSTR fileName);
	HRESULT InitGeometry(ID3D11Device *device, vector<float> &points, float lineWidth, XMFLOAT4 color, float depth);

	void RenderLines(ID3D11DeviceContext *painter, Camera2D &camera);
	void RenderPoints(ID3D11DeviceContext *painter, Camera2D &camera);
	void Render(ID3D11DeviceContext *painter, Camera2D &camera);

	void ReleaseGeometry();
	void ReleaseFX();
	void Release();
};

