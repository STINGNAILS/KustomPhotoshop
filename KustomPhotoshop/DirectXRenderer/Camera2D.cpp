#include "stdafx.h"
#include "Camera2D.h"



Camera2D::Camera2D()
{
	SetOrthographicView(8.0, 1.5f);
	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	CalculateView();
}


Camera2D::~Camera2D()
{

}


void Camera2D::SetPosition(XMFLOAT3 position_)
{
	position = position_;
	CalculateView();
}


void Camera2D::SetZoom(float scale)
{
	XMFLOAT4X4 zoom;

	zoom(0, 0) = scale;
	zoom(1, 0) = 0.0f;
	zoom(2, 0) = 0.0f;
	zoom(3, 0) = 0.0f;

	zoom(0, 1) = 0.0f;
	zoom(1, 1) = scale;
	zoom(2, 1) = 0.0f;
	zoom(3, 1) = 0.0f;

	zoom(0, 2) = 0.0f;
	zoom(1, 2) = 0.0f;
	zoom(2, 2) = 1.0f;
	zoom(3, 2) = 0.0f;

	zoom(0, 3) = 0.0f;
	zoom(1, 3) = 0.0f;
	zoom(2, 3) = 0.0f;
	zoom(3, 3) = 1.0f;

	zoomM = XMLoadFloat4x4(&zoom);
}


void Camera2D::SetOrthographicView(float height, float ratio)
{
	XMMATRIX projM;
	projM = XMMatrixOrthographicLH(ratio * height, height, 0.0f, 10.0f);
	XMStoreFloat4x4(&proj, projM);
}


const XMMATRIX Camera2D::View()
{
	return XMLoadFloat4x4(&view);
}


const XMMATRIX Camera2D::Proj()
{
	return XMLoadFloat4x4(&proj);
}


const XMMATRIX Camera2D::ViewProj()
{
	return XMMatrixMultiply(View(), XMMatrixMultiply(Proj(), zoomM));
}


const XMFLOAT3 Camera2D::Position()
{
	return position;
}


void Camera2D::Move(float x, float y)
{
	position.x += speed * x;
	position.y += speed * y;
}


void Camera2D::CalculateView()
{
	view(0, 0) = 1.0f;
	view(1, 0) = 0.0f;
	view(2, 0) = 0.0f;
	view(3, 0) = -position.x;

	view(0, 1) = 0.0f;
	view(1, 1) = 1.0f;
	view(2, 1) = 0.0f;
	view(3, 1) = -position.y;

	view(0, 2) = 0.0f;
	view(1, 2) = 0.0f;
	view(2, 2) = 1.0f;
	view(3, 2) = -position.z;

	view(0, 3) = 0.0f;
	view(1, 3) = 0.0f;
	view(2, 3) = 0.0f;
	view(3, 3) = 1.0f;
}
