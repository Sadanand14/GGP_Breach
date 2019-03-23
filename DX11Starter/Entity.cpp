#include "Entity.h"

using namespace DirectX;

Entity::Entity(char* fileName, ID3D11Device* device ,Material* mat)
{
	// Setting the world as the identity
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, W);

	meshObject = new Mesh(fileName, device);
	material = mat;

	// Initial values
	position = XMFLOAT3(0,0,0);
	scale = XMFLOAT3(1, 1, 1);
	rotQuat = XMVectorSet(0, 0, 0, 0);
}

Entity::~Entity()
{
	delete meshObject;
}

void Entity::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void Entity::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}

void Entity::SetRotation(float x, float y, float z)
{
	XMVECTOR rotate = XMVectorSet(x, y, z, 0);
	rotQuat = XMQuaternionRotationRollPitchYawFromVector(rotate);
}

void Entity::SetWorldMatrix(DirectX::XMFLOAT4X4 WM)
{
	worldMatrix = WM;
}

DirectX::XMFLOAT4X4 Entity::CalculateWorldMatrix(Entity * object)
{
	// Create transformation matrices
	XMMATRIX trans = XMMatrixTranslation(object->GetPosition().x, object->GetPosition().y, object->GetPosition().z);
	XMMATRIX rot = XMMatrixRotationQuaternion(object->GetRotation());
	XMMATRIX scale = XMMatrixScaling(object->GetScale().x, object->GetScale().y, object->GetScale().z);

	XMFLOAT4X4 holder;
	XMStoreFloat4x4(&holder, XMMatrixTranspose(scale * rot * trans));

	return holder;
}

void Entity::PrepareShader(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projMatrix)
{
	material->GetVertexShader()->SetMatrix4x4("view", viewMatrix);
	material->GetVertexShader()->SetMatrix4x4("projection", projMatrix);
	material->GetVertexShader()->SetMatrix4x4("world", worldMatrix);

	material->GetPixelShader()->SetShaderResourceView("res", material->GetShaderResourceView());
	material->GetPixelShader()->SetShaderResourceView("normalMap", material->GetNormalResourceView());
	material->GetPixelShader()->SetSamplerState("state", material->GetSamplerState());

	material->GetVertexShader()->CopyAllBufferData();
	material->GetPixelShader()->CopyAllBufferData();

	material->GetVertexShader()->SetShader();
	material->GetPixelShader()->SetShader();
}

DirectX::XMFLOAT4X4 Entity::GetWorldMatrix()
{
	return worldMatrix;
}

DirectX::XMFLOAT3 Entity::GetPosition()
{
	return position;
}

DirectX::XMFLOAT3 Entity::GetScale()
{
	return scale;
}

DirectX::XMVECTOR Entity::GetRotation()
{
	return rotQuat;
}