#include "stdafx.h"
#include "BoundingBox.h"


struct BoundingBoxVertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
};


BoundingBox::BoundingBox()
{
}


BoundingBox::~BoundingBox()
{
}


HRESULT BoundingBox::InitFX(ID3D11Device *device, LPCWSTR fileName)
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
	bufferDesc.ByteWidth = sizeof(BoundingBoxCB);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	hr = device->CreateBuffer(&bufferDesc, 0, &boundingBoxConstantBuffer);
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


HRESULT BoundingBox::InitGeometry(ID3D11Device *device, XMFLOAT3 minPoint, XMFLOAT3 maxPoint)
{
	HRESULT hr = S_OK;

	const XMFLOAT3 p1 = minPoint;
	const XMFLOAT3 p2 = XMFLOAT3(maxPoint.x, minPoint.y, 0.0f);
	const XMFLOAT3 p3 = XMFLOAT3(minPoint.x, maxPoint.y, 0.0f);
	const XMFLOAT3 p4 = maxPoint;

	vector<BoundingBoxVertex> boundingBoxVertices(6);

	boundingBoxVertices[0].pos = p3;
	boundingBoxVertices[0].color = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	boundingBoxVertices[1].pos = p4;
	boundingBoxVertices[1].color = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	boundingBoxVertices[2].pos = p1;
	boundingBoxVertices[2].color = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	boundingBoxVertices[3].pos = p4;
	boundingBoxVertices[3].color = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	boundingBoxVertices[4].pos = p2;
	boundingBoxVertices[4].color = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	boundingBoxVertices[5].pos = p1;
	boundingBoxVertices[5].color = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	D3D11_BUFFER_DESC vbDesc;
	vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vbDesc.ByteWidth = sizeof(BoundingBoxVertex) * boundingBoxVertices.size();
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vbData;
	vbData.pSysMem = &boundingBoxVertices[0];

	hr = device->CreateBuffer(&vbDesc, &vbData, &vertexBuffer);

	boundaries = XMFLOAT4(minPoint.x, minPoint.y, maxPoint.x, maxPoint.y);

	return hr;
}


void BoundingBox::SetZoom(float currentZoom_)
{
	currentZoom = currentZoom_;
}


void BoundingBox::Render(ID3D11DeviceContext *painter, Camera2D &camera)
{
	UINT stride = sizeof(BoundingBoxVertex);
	UINT offset = 0;

	painter->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	painter->IASetInputLayout(inputLayout);
	painter->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	painter->RSSetState(basicRasterizerState);

	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	painter->OMSetBlendState(transparentBlendState, blendFactors, 0xffffffff);

	XMMATRIX viewProjM = camera.ViewProj();

	BoundingBoxCB boundingBoxCB;
	XMStoreFloat4x4(&boundingBoxCB.viewProj, XMMatrixTranspose(viewProjM));
	boundingBoxCB.boundaries = boundaries;
	boundingBoxCB.currentZoom = currentZoom;

	painter->UpdateSubresource(boundingBoxConstantBuffer, 0, 0, &boundingBoxCB, 0, 0);

	painter->VSSetShader(vertexShader, 0, 0);
	painter->VSSetConstantBuffers(0, 1, &boundingBoxConstantBuffer);

	painter->PSSetShader(pixelShader, 0, 0);
	painter->PSSetConstantBuffers(0, 1, &boundingBoxConstantBuffer);

	painter->Draw(6, 0);

	painter->RSSetState(0);
}


void BoundingBox::ReleaseFX()
{
	if (inputLayout) inputLayout->Release();
	if (boundingBoxConstantBuffer) boundingBoxConstantBuffer->Release();
	if (basicRasterizerState) basicRasterizerState->Release();
	if (vertexShader) vertexShader->Release();
	if (pixelShader) pixelShader->Release();
}


void BoundingBox::ReleaseGeometry()
{
	if (vertexBuffer) vertexBuffer->Release();
}


void BoundingBox::Release()
{
	ReleaseGeometry();
	ReleaseFX();
}