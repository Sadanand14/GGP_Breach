#include "Texture.h"
using namespace DirectX;


Texture::Texture(ID3D11Device * dev, ID3D11DeviceContext * devContext, const wchar_t * fileName, ID3D11ShaderResourceView ** texture)
{
	CreateWICTextureFromFile(dev, devContext, fileName, 0, texture);

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	shaderResource = *texture;

	dev->CreateSamplerState(&samplerDesc, &samplerState);
}

Texture::~Texture()
{
	// deleting shader resource
	shaderResource->Release();
	samplerState->Release();
}

ID3D11ShaderResourceView * Texture::GetShaderResourceView()
{
	return shaderResource;
}

ID3D11SamplerState * Texture::GetSamplerState()
{
	return samplerState;
}
