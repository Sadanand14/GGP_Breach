#include "Camera.h"

using namespace DirectX;

Camera::Camera()
{
	// Setting intial values
	camPos = XMVectorSet(0, 0, -5, 0);
	camRotX = 0.0f;
	camRotY = 0.0f;
	cameraSpeed = 13.5f;
	forward = XMVectorSet(0, 0, 1, 0);
	up = XMVectorSet(0, 1, 0, 0);
}

Camera::~Camera()
{
}

void Camera::Update(float dt)
{
	// Updating vector for rotation
	XMVECTOR rotateHolder = XMVectorSet(camRotX, camRotY, 0, 0);
	XMVECTOR rotateQuat = XMQuaternionRotationRollPitchYawFromVector(rotateHolder);

	forward = XMVector3Rotate(XMVectorSet(0, 0, 1, 0),rotateQuat);

	Movement(dt);

	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(XMMatrixLookToLH(camPos, forward, up)));
}

void Camera::Movement(float dt)
{
	// moving based on cameraspeed and deltatime
	if (GetAsyncKeyState('W') & 0x8000) {
		camPos += (forward * cameraSpeed) * dt;
	}

	if (GetAsyncKeyState('S') & 0x8000) {
		camPos += -(forward * cameraSpeed) * dt;
	}

	if (GetAsyncKeyState('A') & 0x8000) {
		camPos += (XMVector3Cross(forward, up) * cameraSpeed) * dt;
	}

	if (GetAsyncKeyState('D') & 0x8000) {
		camPos += -(XMVector3Cross(forward, up) * cameraSpeed) * dt;
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		camPos += (up * cameraSpeed) * dt;
	}

	if (GetAsyncKeyState('X') & 0x8000) {
		camPos += -(up * cameraSpeed) * dt;
	}
}

DirectX::XMFLOAT4X4 Camera::GetViewMatrix()
{
	return viewMatrix;
}

DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projMatrix;
}

DirectX::XMVECTOR Camera::GetCameraPostion()
{
	return camPos;
}

DirectX::XMVECTOR Camera::GetCameraDirection()
{
	return camDir;
}

void Camera::UpdateProjectionMatrix(float aspect)
{
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		aspect,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}

void Camera::RotateCamera(float x, float y)
{
	// cases to make sure x and y values don't get to high
	if (camRotX >= 6.25f) {
		camRotX = 0.0f;
	}
	else if (camRotX <= -6.25f) {
		camRotX = -0.0f;
	}
	else {
		camRotX += y;
	}
	
	if (camRotY >= 6.25f) {
		camRotY = 0.0f;
	}
	else if (camRotY <= -6.25f) {
		camRotY = -0.0f;
	}
	else {
		camRotY += x;
	}
}

void Camera::SetCameraPostion(DirectX::XMVECTOR newPos)
{
	camPos = newPos;
}

void Camera::SetCameraDirection(DirectX::XMVECTOR newDir)
{
	camDir = newDir;
}

float Camera::GetCameraRotationX()
{
	return camRotX;
}

float Camera::GetCameraRotationY()
{
	return camRotY;
}

void Camera::SetCameraRotationX(float newX)
{
	camRotX = newX;
}

void Camera::SetCameraRotationY(float newY)
{
	camRotY = newY;
}


