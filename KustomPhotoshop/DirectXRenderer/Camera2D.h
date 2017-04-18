#pragma once

#include "DirectXDependencies.h"
#include <DirectXMath.h>

using namespace DirectX;

class Camera2D
{
	private:

	XMFLOAT3 position;

	XMFLOAT4X4 view;
	XMFLOAT4X4 proj;

	XMMATRIX zoomInM;
	XMMATRIX zoomOutM;

	double speed;

	public:

	Camera2D();
	~Camera2D();

	void SetPosition(XMFLOAT3 position_);
	void SetOrthographicView(float height, float ratio);

	const XMMATRIX View();
	const XMMATRIX Proj();
	const XMMATRIX ViewProj();

	const XMFLOAT3 Position();

	void Move(float x, float y);

	void ZoomIn();
	void ZoomOut();

	void CalculateView();
};

