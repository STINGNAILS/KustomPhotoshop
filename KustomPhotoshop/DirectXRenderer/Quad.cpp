#include "stdafx.h"
#include "Quad.h"


struct QuadVertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
};


Quad::Quad()
{

}


Quad::~Quad()
{

}


HRESULT Quad::InitFX(ID3D11Device *device, LPCWSTR fileName)
{
	HRESULT hr = S_OK;

	ID3DBlob *shaderBlob = 0;

	hr = DirectXGraphics::CompileShaderFromFile(fileName, "VS", "vs_5_0", &shaderBlob);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &vertexShader);
	if (FAILED(hr))
	{
		shaderBlob->Release();
		return hr;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	hr = device->CreateInputLayout(layout, ARRAYSIZE(layout), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &inputLayout);
	shaderBlob->Release();
	if (FAILED(hr))
	{
		return hr;
	}

	hr = DirectXGraphics::CompileShaderFromFile(fileName, "PS", "ps_5_0", &shaderBlob);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &pixelShader);
	shaderBlob->Release();
	if (FAILED(hr))
	{
		return hr;
	}

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(QuadCB);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	hr = device->CreateBuffer(&bufferDesc, 0, &constantBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;

	hr = device->CreateRasterizerState(&rasterizerDesc, &basicRasterizerState);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = borderLine.InitFX(device, L"PolyLineShader.hlsl");

	return hr;
}


HRESULT Quad::InitGeometry(ID3D11Device *device, XMFLOAT3 v1, XMFLOAT3 v2, XMFLOAT3 v3, XMFLOAT3 v4, XMFLOAT4 color, XMFLOAT4 borderColor, float width_)
{
	HRESULT hr = S_OK;

	vector<QuadVertex> quadVertices(6);

	quadVertices[0].pos = v1;
	quadVertices[0].color = color;

	quadVertices[1].pos = v2;
	quadVertices[1].color = color;

	quadVertices[2].pos = v3;
	quadVertices[2].color = color;

	quadVertices[3].pos = v3;
	quadVertices[3].color = color;

	quadVertices[4].pos = v4;
	quadVertices[4].color = color;

	quadVertices[5].pos = v1;
	quadVertices[5].color = color;

	D3D11_BUFFER_DESC vbDesc;
	vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vbDesc.ByteWidth = sizeof(QuadVertex) * quadVertices.size();
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vbData;
	vbData.pSysMem = &quadVertices[0];

	hr = device->CreateBuffer(&vbDesc, &vbData, &vertexBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	vector<float> points(10);
	points[0] = v1.x;
	points[1] = v1.y;
	points[2] = v2.x;
	points[3] = v2.y;
	points[4] = v3.x;
	points[5] = v3.y;
	points[6] = v4.x;
	points[7] = v4.y;
	points[8] = v1.x;
	points[9] = v1.y;

	hr = borderLine.InitGeometry(device, points, width_, borderColor);

	return hr;
}


void Quad::Render(ID3D11DeviceContext *painter, Camera2D &camera)
{
	UINT stride = sizeof(QuadVertex);
	UINT offset = 0;

	painter->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	painter->IASetInputLayout(inputLayout);
	painter->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	painter->RSSetState(basicRasterizerState);

	XMMATRIX viewProjM = camera.ViewProj();

	QuadCB quadCB;
	XMStoreFloat4x4(&quadCB.viewProj, XMMatrixTranspose(viewProjM));

	painter->UpdateSubresource(constantBuffer, 0, 0, &quadCB, 0, 0);

	painter->VSSetShader(vertexShader, 0, 0);
	painter->VSSetConstantBuffers(0, 1, &constantBuffer);

	painter->PSSetShader(pixelShader, 0, 0);
	painter->PSSetConstantBuffers(0, 1, &constantBuffer);

	painter->Draw(6, 0);

	painter->RSSetState(0);

	borderLine.Render(painter, camera);
}


void Quad::ReleaseFX()
{
	if (inputLayout) inputLayout->Release();
	if (constantBuffer) constantBuffer->Release();
	if (basicRasterizerState) basicRasterizerState->Release();
	if (vertexShader) vertexShader->Release();
	if (pixelShader) pixelShader->Release();

	borderLine.ReleaseFX();
}


void Quad::ReleaseGeometry()
{
	if (vertexBuffer) vertexBuffer->Release();

	borderLine.ReleaseGeometry();
}


void Quad::Release()
{
	ReleaseGeometry();
	ReleaseFX();
}