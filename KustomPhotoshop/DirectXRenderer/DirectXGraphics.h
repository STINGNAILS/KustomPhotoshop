#pragma once

#include "DirectXDependencies.h"

class DirectXGraphics
{
	private:

	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Device *device;
	ID3D11DeviceContext *painter;
	IDXGISwapChain *swapChain;
	ID3D11RenderTargetView *renderTargetView;
	ID3D11DepthStencilView *depthStencilView;
	ID3D11Texture2D *depthStencilBuffer;

	D3D11_VIEWPORT viewport;

	public:

	DirectXGraphics();
	~DirectXGraphics();

	ID3D11Device *GetDevice();
	ID3D11DeviceContext *GetPainter();

	HRESULT Init(HWND hwnd);

	void BeginRender();
	void FinishRender();

	void Release();

	static HRESULT CompileShaderFromFile(LPCWSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob **ppBlobOut);
};

