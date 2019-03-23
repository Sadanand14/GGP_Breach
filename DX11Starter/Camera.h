#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "DXCore.h"

class Camera
{
public:
	Camera();
	~Camera();

	void Update(float dt);

	void UpdateProjectionMatrix(float aspect);

	void Movement(float dt);

	void RotateCamera(float x, float y);

	// Getter methods
	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();

	DirectX::XMVECTOR GetCameraPostion();
	DirectX::XMVECTOR GetCameraDirection();

	float GetCameraRotationX();
	float GetCameraRotationY();

	// Setter methods
	void SetCameraPostion(DirectX::XMVECTOR newPos);
	void SetCameraDirection(DirectX::XMVECTOR newDir);

	void SetCameraRotationX(float newX);
	void SetCameraRotationY(float newY);

	

private:
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projMatrix;

	DirectX::XMVECTOR camPos;
	DirectX::XMVECTOR camDir;

	DirectX::XMVECTOR forward;
	DirectX::XMVECTOR up;

	float camRotX;
	float camRotY;

	float cameraSpeed;
};

