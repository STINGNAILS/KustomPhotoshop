#include "Camera2D.h"



Camera2D::Camera2D()
{
	SetOrthographicView(8.0, 1.5f);
	CalculateView();

	XMFLOAT4X4 zoomIn;

	zoomIn(0, 0) = 1.176471f;
	zoomIn(1, 0) = 0.0f;
	zoomIn(2, 0) = 0.0f;
	zoomIn(3, 0) = 0.0f;

	zoomIn(0, 1) = 0.0f;
	zoomIn(1, 1) = 1.176471f;
	zoomIn(2, 1) = 0.0f;
	zoomIn(3, 1) = 0.0f;

	zoomIn(0, 2) = 0.0f;
	zoomIn(1, 2) = 0.0f;
	zoomIn(2, 2) = 1.0f;
	zoomIn(3, 2) = 0.0f;

	zoomIn(0, 3) = 0.0f;
	zoomIn(1, 3) = 0.0f;
	zoomIn(2, 3) = 0.0f;
	zoomIn(3, 3) = 1.0f;

	zoomInM = XMLoadFloat4x4(&zoomIn);

	XMFLOAT4X4 zoomOut;

	zoomOut(0, 0) = 0.85f;
	zoomOut(1, 0) = 0.0f;
	zoomOut(2, 0) = 0.0f;
	zoomOut(3, 0) = 0.0f;

	zoomOut(0, 1) = 0.0f;
	zoomOut(1, 1) = 0.85f;
	zoomOut(2, 1) = 0.0f;
	zoomOut(3, 1) = 0.0f;

	zoomOut(0, 2) = 0.0f;
	zoomOut(1, 2) = 0.0f;
	zoomOut(2, 2) = 1.0f;
	zoomOut(3, 2) = 0.0f;

	zoomOut(0, 3) = 0.0f;
	zoomOut(1, 3) = 0.0f;
	zoomOut(2, 3) = 0.0f;
	zoomOut(3, 3) = 1.0f;

	zoomOutM = XMLoadFloat4x4(&zoomOut);

	speed = 1.0;
}


Camera2D::~Camera2D()
{

}


void Camera2D::SetPosition(XMFLOAT3 position_)
{
	position = position_;
	CalculateView();
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
	return XMMatrixMultiply(View(), Proj());
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


void Camera2D::ZoomIn()
{
	XMMATRIX projM = XMMatrixMultiply(Proj(), zoomInM);
	XMStoreFloat4x4(&proj, projM);

	speed *= 0.85;
}


void Camera2D::ZoomOut()
{
	XMMATRIX projM = XMMatrixMultiply(Proj(), zoomOutM);
	XMStoreFloat4x4(&proj, projM);

	speed *= 1.176471;
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
