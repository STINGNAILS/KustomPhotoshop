#include "stdafx.h"
#include "Triangle.h"


struct TriangleVertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
};


Triangle::Triangle()
{

}


Triangle::~Triangle()
{

}


HRESULT Triangle::InitFX(ID3D11Device *device, LPCWSTR fileName)
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
	bufferDesc.ByteWidth = sizeof(TriangleCB);
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


HRESULT Triangle::InitGeometry(ID3D11Device *device, XMFLOAT3 v1, XMFLOAT3 v2, XMFLOAT3 v3, XMFLOAT4 color, XMFLOAT4 borderColor, float width_)
{
	HRESULT hr = S_OK;

	vector<TriangleVertex> triangleVertices(3);

	triangleVertices[0].pos = v1;
	triangleVertices[0].color = color;

	triangleVertices[1].pos = v2;
	triangleVertices[1].color = color;

	triangleVertices[2].pos = v3;
	triangleVertices[2].color = color;

	D3D11_BUFFER_DESC vbDesc;
	vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vbDesc.ByteWidth = sizeof(TriangleVertex) * triangleVertices.size();
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vbData;
	vbData.pSysMem = &triangleVertices[0];

	hr = device->CreateBuffer(&vbDesc, &vbData, &vertexBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	vector<float> points(8);
	points[0] = v1.x;
	points[1] = v1.y;
	points[2] = v2.x;
	points[3] = v2.y;
	points[4] = v3.x;
	points[5] = v3.y;
	points[6] = v1.x;
	points[7] = v1.y;

	hr = borderLine.InitGeometry(device, points, width_, borderColor);

	return hr;
}


void Triangle::Render(ID3D11DeviceContext *painter, Camera2D &camera)
{
	UINT stride = sizeof(TriangleVertex);
	UINT offset = 0;

	painter->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	painter->IASetInputLayout(inputLayout);
	painter->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	painter->RSSetState(basicRasterizerState);

	XMMATRIX viewProjM = camera.ViewProj();

	TriangleCB triangleCB;
	XMStoreFloat4x4(&triangleCB.viewProj, XMMatrixTranspose(viewProjM));

	painter->UpdateSubresource(constantBuffer, 0, 0, &triangleCB, 0, 0);

	painter->VSSetShader(vertexShader, 0, 0);
	painter->VSSetConstantBuffers(0, 1, &constantBuffer);

	painter->PSSetShader(pixelShader, 0, 0);
	painter->PSSetConstantBuffers(0, 1, &constantBuffer);

	painter->Draw(3, 0);

	painter->RSSetState(0);

	borderLine.Render(painter, camera);
}


void Triangle::ReleaseFX()
{
	if (inputLayout) inputLayout->Release();
	if (constantBuffer) constantBuffer->Release();
	if (basicRasterizerState) basicRasterizerState->Release();
	if (vertexShader) vertexShader->Release();
	if (pixelShader) pixelShader->Release();

	borderLine.ReleaseFX();
}


void Triangle::ReleaseGeometry()
{
	if (vertexBuffer) vertexBuffer->Release();

	borderLine.ReleaseGeometry();
}


void Triangle::Release()
{
	ReleaseGeometry();
	ReleaseFX();
}
