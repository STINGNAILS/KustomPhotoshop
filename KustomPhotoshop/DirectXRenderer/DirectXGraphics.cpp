#include "stdafx.h"
#include "DirectXGraphics.h"


DirectXGraphics::DirectXGraphics()
{
	driverType = D3D_DRIVER_TYPE_NULL;
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	device = NULL;
	painter = NULL;
	swapChain = NULL;
	renderTargetView = NULL;
}


DirectXGraphics::~DirectXGraphics()
{

}


ID3D11Device *DirectXGraphics::GetDevice()
{
	return device;
}


ID3D11DeviceContext * DirectXGraphics::GetPainter()
{
	return painter;
}


HRESULT DirectXGraphics::Init(HWND hwnd)
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
	#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(0, driverType, 0, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, &featureLevel, &painter);
		if (SUCCEEDED(hr))
		{
			break;
		}
	}
	if (FAILED(hr))
	{
		return hr;
	}
	
	hr = Resize(hwnd);

	return hr;
}


HRESULT DirectXGraphics::Resize(HWND hwnd)
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	if(renderTargetView) renderTargetView->Release();

	hr = swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	if (FAILED(hr))
	{
		return hr;
	}

	ID3D11Texture2D *backBuffer = 0;
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = device->CreateRenderTargetView(backBuffer, 0, &renderTargetView);
	backBuffer->Release();
	if (FAILED(hr))
	{
		return hr;
	}

	viewport.Width = (FLOAT)width;
	viewport.Height = (FLOAT)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	return hr;
}


void DirectXGraphics::BeginRender()
{
	const float clearColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	painter->ClearRenderTargetView(renderTargetView, clearColor);

	painter->OMSetRenderTargets(1, &renderTargetView, 0);
	painter->RSSetViewports(1, &viewport);
}


void DirectXGraphics::FinishRender()
{
	swapChain->Present(0, 0);
}


HRESULT DirectXGraphics::CompileShaderFromFile(LPCWSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob **ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	#if defined(DEBUG) || defined (_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	#endif

	ID3DBlob *errorBlob;
	hr = D3DCompileFromFile(szFileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, szEntryPoint, szShaderModel, dwShaderFlags, 0, ppBlobOut, &errorBlob);
	if (FAILED(hr))
	{
		if (errorBlob != NULL)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		}
		if (errorBlob)
		{
			errorBlob->Release();
		}
		return hr;
	}
	if (errorBlob)
	{
		errorBlob->Release();
	}

	return S_OK;
}

void DirectXGraphics::Release()
{
	if (painter) painter->ClearState();

	if (device) device->Release();
	if (painter) painter->Release();
	if (swapChain) swapChain->Release();
	if (renderTargetView) renderTargetView->Release();
}