#include "stdafx.h"
#include "Triangle.h"


struct TriangleVertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
	XMFLOAT4 borderColor;
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
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0},
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

	hr = device->CreateBuffer(&bufferDesc, 0, &triangleConstantBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;

	hr = device->CreateRasterizerState(&rasterizerDesc, &basicRasterizerState);

	return hr;
}


HRESULT Triangle::InitGeometry(ID3D11Device *device, XMFLOAT3 v1_, XMFLOAT3 v2_, XMFLOAT3 v3_, XMFLOAT4 color, XMFLOAT4 borderColor, float width_)
{
	width = width_;
	v1 = v1_;
	v2 = v2_;
	v3 = v3_;

	HRESULT hr = S_OK;

	vector<TriangleVertex> triangleVertices(36);

	triangleVertices[0].pos = v1;
	triangleVertices[0].color = color;
	triangleVertices[0].borderColor = borderColor;

	triangleVertices[1].pos = v2;
	triangleVertices[1].color = color;
	triangleVertices[1].borderColor = borderColor;

	triangleVertices[2].pos = v3;
	triangleVertices[2].color = color;
	triangleVertices[2].borderColor = borderColor;

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
	triangleCB.v1v2 = XMFLOAT4(v1.x, v1.y, v2.x, v2.y);
	triangleCB.v3Width = XMFLOAT4(v3.x, v3.y, width, 0.0f);

	painter->UpdateSubresource(triangleConstantBuffer, 0, 0, &triangleCB, 0, 0);

	painter->VSSetShader(vertexShader, 0, 0);
	painter->VSSetConstantBuffers(0, 1, &triangleConstantBuffer);

	painter->PSSetShader(pixelShader, 0, 0);
	painter->PSSetConstantBuffers(0, 1, &triangleConstantBuffer);

	painter->Draw(3, 0);

	painter->RSSetState(0);
}


void Triangle::ReleaseFX()
{
	if (inputLayout) inputLayout->Release();
	if (triangleConstantBuffer) triangleConstantBuffer->Release();
	if (basicRasterizerState) basicRasterizerState->Release();
	if (vertexShader) vertexShader->Release();
	if (pixelShader) pixelShader->Release();
}


void Triangle::ReleaseGeometry()
{
	if (vertexBuffer) vertexBuffer->Release();
}


void Triangle::Release()
{
	ReleaseGeometry();
	ReleaseFX();
}
