#include "stdafx.h"
#include "PolyLine.h"


PolyLine::PolyLine()
{

}


PolyLine::~PolyLine()
{

}


struct LineVertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
	XMFLOAT2 uv;
};


HRESULT PolyLine::InitGeometry(ID3D11Device *device, vector<float> &points, float lineWidth, XMFLOAT4 color)
{
	HRESULT hr;

	lineVertexNum = points.size() * 3 - 6;
	pointVertexNum = points.size() * 3;

	if (lineVertexNum == 0)
	{
		return S_OK;
	}

	vector<LineVertex> lineVertices;
	lineVertices.resize(lineVertexNum);

	for (int i = 0; i < points.size() - 2; i += 2)
	{
		XMFLOAT2 a = XMFLOAT2(points[i], points[i + 1]);
		XMFLOAT2 b = XMFLOAT2(points[i + 2], points[i + 3]);
		XMFLOAT2 v = XMFLOAT2(b.x - a.x, b.y - a.y);
		
		int signx = v.y >= 0 ? 1 : -1;
		int signy = v.x >= 0 ? -1 : 1;

		XMFLOAT2 r = XMFLOAT2(lineWidth / 2 * signx * sqrt(v.y * v.y / (v.x * v.x + v.y * v.y)), lineWidth / 2 * signy * sqrt(v.x * v.x / (v.x * v.x + v.y * v.y)));

		lineVertices[i * 3].pos = XMFLOAT3(a.x + r.x, a.y + r.y, 0.0f);
		lineVertices[i * 3].color = color;
		lineVertices[i * 3].uv = XMFLOAT2(0.0f, 0.0f);

		lineVertices[i * 3 + 1].pos = XMFLOAT3(a.x - r.x, a.y - r.y, 0.0f);
		lineVertices[i * 3 + 1].color = color;
		lineVertices[i * 3 + 1].uv = XMFLOAT2(1.0f, 0.0f);

		lineVertices[i * 3 + 2].pos = XMFLOAT3(b.x + r.x, b.y + r.y, 0.0f);
		lineVertices[i * 3 + 2].color = color;
		lineVertices[i * 3 + 2].uv = XMFLOAT2(0.0f, 1.0f);

		lineVertices[i * 3 + 3].pos = XMFLOAT3(a.x - r.x, a.y - r.y, 0.0f);
		lineVertices[i * 3 + 3].color = color;
		lineVertices[i * 3 + 3].uv = XMFLOAT2(1.0f, 0.0f);

		lineVertices[i * 3 + 4].pos = XMFLOAT3(b.x - r.x, b.y - r.y, 0.0f);
		lineVertices[i * 3 + 4].color = color;
		lineVertices[i * 3 + 4].uv = XMFLOAT2(1.0f, 1.0f);

		lineVertices[i * 3 + 5].pos = XMFLOAT3(b.x + r.x, b.y + r.y, 0.0f);
		lineVertices[i * 3 + 5].color = color;
		lineVertices[i * 3 + 5].uv = XMFLOAT2(0.0f, 1.0f);
	}

	D3D11_BUFFER_DESC vbDesc;
	vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vbDesc.ByteWidth = sizeof(LineVertex) * lineVertices.size();
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vbData;
	vbData.pSysMem = &lineVertices[0];
	hr = device->CreateBuffer(&vbDesc, &vbData, &vertexBufferLines);
	if (FAILED(hr))
	{
		return hr;
	}

	vector<LineVertex> pointVertices;
	pointVertices.resize(pointVertexNum);

	for (int i = 0; i < points.size(); i += 2)
	{
		XMFLOAT2 point = XMFLOAT2(points[i], points[i + 1]);

		pointVertices[i * 3].pos = XMFLOAT3(point.x - lineWidth / 2, point.y + lineWidth / 2, 0.0f);
		pointVertices[i * 3].color = color;
		pointVertices[i * 3].uv = XMFLOAT2(-1.0f, -1.0f);

		pointVertices[i * 3 + 1].pos = XMFLOAT3(point.x + lineWidth / 2, point.y + lineWidth / 2, 0.0f);
		pointVertices[i * 3 + 1].color = color;
		pointVertices[i * 3 + 1].uv = XMFLOAT2(1.0f, -1.0f);

		pointVertices[i * 3 + 2].pos = XMFLOAT3(point.x - lineWidth / 2, point.y - lineWidth / 2, 0.0f);
		pointVertices[i * 3 + 2].color = color;
		pointVertices[i * 3 + 2].uv = XMFLOAT2(-1.0f, 1.0f);

		pointVertices[i * 3 + 3].pos = XMFLOAT3(point.x + lineWidth / 2, point.y + lineWidth / 2, 0.0f);
		pointVertices[i * 3 + 3].color = color;
		pointVertices[i * 3 + 3].uv = XMFLOAT2(1.0f, -1.0f);

		pointVertices[i * 3 + 4].pos = XMFLOAT3(point.x + lineWidth / 2, point.y - lineWidth / 2, 0.0f);
		pointVertices[i * 3 + 4].color = color;
		pointVertices[i * 3 + 4].uv = XMFLOAT2(1.0f, 1.0f);

		pointVertices[i * 3 + 5].pos = XMFLOAT3(point.x - lineWidth / 2, point.y - lineWidth / 2, 0.0f);
		pointVertices[i * 3 + 5].color = color;
		pointVertices[i * 3 + 5].uv = XMFLOAT2(-1.0f, 1.0f);
	}

	vbDesc.ByteWidth = sizeof(LineVertex) * pointVertices.size();

	vbData.pSysMem = &pointVertices[0];
	hr = device->CreateBuffer(&vbDesc, &vbData, &vertexBufferPoints);
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}


HRESULT PolyLine::InitFX(ID3D11Device *device, LPCWSTR fileName)
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
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	hr = device->CreateInputLayout(layout, ARRAYSIZE(layout), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &inputLayout);
	shaderBlob->Release();
	if (FAILED(hr))
	{
		return hr;
	}

	hr = DirectXGraphics::CompileShaderFromFile((LPCWSTR)fileName, "PS_LINES", "ps_5_0", &shaderBlob);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &pixelShaderLines);
	shaderBlob->Release();
	if (FAILED(hr))
	{
		return hr;
	}

	hr = DirectXGraphics::CompileShaderFromFile((LPCWSTR)fileName, "PS_POINTS", "ps_5_0", &shaderBlob);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &pixelShaderPoints);
	shaderBlob->Release();
	if (FAILED(hr))
	{
		return hr;
	}

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(PolyLineCB);
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


void PolyLine::RenderLines(ID3D11DeviceContext *painter, Camera2D &camera)
{
	if (lineVertexNum <= 0)
	{
		return;
	}

	UINT stride = sizeof(LineVertex);
	UINT offset = 0;

	painter->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	painter->IASetInputLayout(inputLayout);
	painter->IASetVertexBuffers(0, 1, &vertexBufferLines, &stride, &offset);

	painter->RSSetState(basicRasterizerState);

	XMMATRIX viewProjM = camera.ViewProj();

	PolyLineCB polyLineCB;
	XMStoreFloat4x4(&polyLineCB.viewProj, XMMatrixTranspose(viewProjM));

	painter->UpdateSubresource(constantBuffer, 0, 0, &polyLineCB, 0, 0);

	painter->VSSetShader(vertexShader, 0, 0);
	painter->VSSetConstantBuffers(0, 1, &constantBuffer);

	painter->PSSetShader(pixelShaderLines, 0, 0);

	painter->Draw(lineVertexNum, 0);

	painter->RSSetState(0);
}


void PolyLine::RenderPoints(ID3D11DeviceContext *painter, Camera2D &camera)
{
	if (pointVertexNum <= 0)
	{
		return;
	}

	UINT stride = sizeof(LineVertex);
	UINT offset = 0;

	painter->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	painter->IASetInputLayout(inputLayout);
	painter->IASetVertexBuffers(0, 1, &vertexBufferPoints, &stride, &offset);

	painter->RSSetState(basicRasterizerState);

	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	painter->OMSetBlendState(transparentBlendState, blendFactors, 0xffffffff);

	XMMATRIX viewProjM = camera.ViewProj();

	PolyLineCB polyLineCB;
	XMStoreFloat4x4(&polyLineCB.viewProj, XMMatrixTranspose(viewProjM));

	painter->UpdateSubresource(constantBuffer, 0, 0, &polyLineCB, 0, 0);

	painter->VSSetShader(vertexShader, 0, 0);
	painter->VSSetConstantBuffers(0, 1, &constantBuffer);

	painter->PSSetShader(pixelShaderPoints, 0, 0);

	painter->Draw(pointVertexNum, 0);

	painter->RSSetState(0);
}


void PolyLine::Render(ID3D11DeviceContext *painter, Camera2D &camera)
{
	RenderLines(painter, camera);
	RenderPoints(painter, camera);
}


void PolyLine::ReleaseGeometry()
{
	if (vertexBufferLines) vertexBufferLines->Release();
	if (vertexBufferPoints) vertexBufferPoints->Release();
}


void PolyLine::ReleaseFX()
{
	if (inputLayout) inputLayout->Release();
	if (constantBuffer) constantBuffer->Release();
	if (basicRasterizerState) basicRasterizerState->Release();
	if (transparentBlendState) transparentBlendState->Release();
	if (vertexShader) vertexShader->Release();
	if (pixelShaderLines) pixelShaderLines->Release();
	if (pixelShaderPoints) pixelShaderPoints->Release();
}


void PolyLine::Release()
{
	ReleaseGeometry();
	ReleaseFX();
}