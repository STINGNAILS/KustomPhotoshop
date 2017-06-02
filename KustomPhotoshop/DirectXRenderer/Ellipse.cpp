#include "stdafx.h"
#include "Ellipse.h"


Ellipse0::Ellipse0()
{

}


Ellipse0::~Ellipse0()
{

}


struct EllipseVertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
	XMFLOAT4 borderColor;
	XMFLOAT2 uv;
};


HRESULT Ellipse0::InitFX(ID3D11Device *device, LPCWSTR fileName)
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
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	hr = device->CreateInputLayout(layout, ARRAYSIZE(layout), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &inputLayout);
	shaderBlob->Release();
	if (FAILED(hr))
	{
		return hr;
	}

	hr = DirectXGraphics::CompileShaderFromFile((LPCWSTR)fileName, "PS", "ps_5_0", &shaderBlob);
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
	bufferDesc.ByteWidth = sizeof(EllipseCB);
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
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;

	hr = device->CreateRasterizerState(&rasterizerDesc, &basicRasterizerState);
	if (FAILED(hr))
	{
		return hr;
	}

	D3D11_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = device->CreateBlendState(&blendDesc, &transparentBlendState);
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}


HRESULT Ellipse0::InitGeometry(ID3D11Device *device, XMFLOAT3 o, float a_, float b_, float angle, XMFLOAT4 color, XMFLOAT4 borderColor, float width_)
{
	HRESULT hr;

	a = a_;
	b = b_;
	width = width_;

	float u = a_ + width_ / 2;
	float v = b_ + width_ / 2;

	XMFLOAT3 v1 = XMFLOAT3(o.x + (-u * cos(angle) + v * sin(angle)), o.y + (-u * sin(angle) - v * cos(angle)), 0.0f);
	XMFLOAT3 v2 = XMFLOAT3(o.x + (u * cos(angle) + v * sin(angle)), o.y + (u * sin(angle) - v * cos(angle)), 0.0f);
	XMFLOAT3 v3 = XMFLOAT3(o.x + (-u * cos(angle) - v * sin(angle)), o.y + (-u * sin(angle) + v * cos(angle)), 0.0f);
	XMFLOAT3 v4 = XMFLOAT3(o.x + (u * cos(angle) - v * sin(angle)), o.y + (u * sin(angle) + v * cos(angle)), 0.0f);

	vector<EllipseVertex> ellipseVertices(6);

	ellipseVertices[0].pos = v1;
	ellipseVertices[0].color = color;
	ellipseVertices[0].borderColor = borderColor;
	ellipseVertices[0].uv = XMFLOAT2(-u, -v);

	ellipseVertices[1].pos = v3;
	ellipseVertices[1].color = color;
	ellipseVertices[1].borderColor = borderColor;
	ellipseVertices[1].uv = XMFLOAT2(-u, v);

	ellipseVertices[2].pos = v4;
	ellipseVertices[2].color = color;
	ellipseVertices[2].borderColor = borderColor;
	ellipseVertices[2].uv = XMFLOAT2(u, v);

	ellipseVertices[3].pos = v4;
	ellipseVertices[3].color = color;
	ellipseVertices[3].borderColor = borderColor;
	ellipseVertices[3].uv = XMFLOAT2(u, v);

	ellipseVertices[4].pos = v2;
	ellipseVertices[4].color = color;
	ellipseVertices[4].borderColor = borderColor;
	ellipseVertices[4].uv = XMFLOAT2(u, -v);

	ellipseVertices[5].pos = v1;
	ellipseVertices[5].color = color;
	ellipseVertices[5].borderColor = borderColor;
	ellipseVertices[5].uv = XMFLOAT2(-u, -v);

	D3D11_BUFFER_DESC vbDesc;
	vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vbDesc.ByteWidth = sizeof(EllipseVertex) * ellipseVertices.size();
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vbData;
	vbData.pSysMem = &ellipseVertices[0];

	hr = device->CreateBuffer(&vbDesc, &vbData, &vertexBuffer);

	return hr;
}


void Ellipse0::Render(ID3D11DeviceContext *painter, Camera2D &camera)
{
	UINT stride = sizeof(EllipseVertex);
	UINT offset = 0;

	painter->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	painter->IASetInputLayout(inputLayout);
	painter->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	painter->RSSetState(basicRasterizerState);

	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	painter->OMSetBlendState(transparentBlendState, blendFactors, 0xffffffff);

	XMMATRIX viewProjM = camera.ViewProj();

	EllipseCB ellipseCB;
	XMStoreFloat4x4(&ellipseCB.viewProj, XMMatrixTranspose(viewProjM));
	ellipseCB.abWidth = XMFLOAT4(a, b, width, 0.0f);

	painter->UpdateSubresource(constantBuffer, 0, 0, &ellipseCB, 0, 0);

	painter->VSSetShader(vertexShader, 0, 0);
	painter->VSSetConstantBuffers(0, 1, &constantBuffer);

	painter->PSSetShader(pixelShader, 0, 0);
	painter->PSSetConstantBuffers(0, 1, &constantBuffer);

	painter->Draw(6, 0);

	painter->RSSetState(0);
}


void Ellipse0::ReleaseGeometry()
{
	if (vertexBuffer) vertexBuffer->Release();
}


void Ellipse0::ReleaseFX()
{
	if (inputLayout) inputLayout->Release();
	if (constantBuffer) constantBuffer->Release();
	if (basicRasterizerState) basicRasterizerState->Release();
	if (transparentBlendState) transparentBlendState->Release();
	if (vertexShader) vertexShader->Release();
	if (pixelShader) pixelShader->Release();
}


void Ellipse0::Release()
{
	ReleaseGeometry();
	ReleaseFX();
}
