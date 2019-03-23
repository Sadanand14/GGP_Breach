#pragma once
#include "Mesh.h"
#include "Material.h"

class Entity
{
public:
	Entity(char* fileName, ID3D11Device* device ,Material* mat);
	~Entity();
	Mesh * meshObject = nullptr;
	Material * material = nullptr;

	// Setters
	void SetPosition(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetWorldMatrix(DirectX::XMFLOAT4X4 WM);
	DirectX::XMFLOAT4X4 CalculateWorldMatrix(Entity* object);
	void PrepareShader(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projMatrix);

	// Getters
	DirectX::XMFLOAT4X4 GetWorldMatrix();
	DirectX::XMFLOAT3 GetPosition ();
	DirectX::XMFLOAT3 GetScale();
	DirectX::XMVECTOR GetRotation();

private:
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	DirectX::XMVECTOR rotQuat;
};

