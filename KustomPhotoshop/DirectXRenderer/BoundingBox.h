#pragma once

#include <vector>
#include "DirectXDependencies.h"
#include "DirectXGraphics.h"
#include "Camera2D.h"

using namespace std;


struct BoundingBoxCB
{
	XMFLOAT4X4 viewProj;
	XMFLOAT4 boundaries;
	float currentZoom;
	float aligner1;
	float aligner2;
	float aligner3;
};


class BoundingBox
{
	private:

	XMFLOAT4 boundaries;
	float currentZoom;

	ID3D11InputLayout *inputLayout;
	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer *boundingBoxConstantBuffer;
	ID3D11RasterizerState *basicRasterizerState;
	ID3D11BlendState *transparentBlendState;
	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *pixelShader;

	public:

	BoundingBox();
	~BoundingBox();

	HRESULT InitFX(ID3D11Device *device, LPCWSTR fileName);
	HRESULT InitGeometry(ID3D11Device *device, XMFLOAT3 minPoint, XMFLOAT3 maxPoint);

	void SetZoom(float currentZoom_);

	void Render(ID3D11DeviceContext *painter, Camera2D &camera);

	void ReleaseFX();
	void ReleaseGeometry();
	void Release();
};

