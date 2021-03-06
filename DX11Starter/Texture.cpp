#include "Texture.h"
using namespace DirectX;


Texture::Texture()
{
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

void Texture::CreateTexure(ID3D11Device * dev, ID3D11DeviceContext * devContext, const wchar_t * fileName, ID3D11ShaderResourceView ** _texture)
{
	CreateWICTextureFromFile(dev, devContext, fileName, 0, _texture);

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	shaderResource = *_texture;

	dev->CreateSamplerState(&samplerDesc, &samplerState);
}

void Texture::CreateCubeMap(ID3D11Device * dev, ID3D11DeviceContext * devContext, const wchar_t * fileName, ID3D11ShaderResourceView ** _cubemap, ID3D11RasterizerState** _rasterState, ID3D11DepthStencilState** _depthState)
{
	// Load the sky box
	CreateDDSTextureFromFile(dev, devContext,fileName, 0, _cubemap);

	// Create a sampler state that holds options for sampling
	// The descriptions should always just be local variables
	D3D11_SAMPLER_DESC samplerDesc = {}; // The {} part zeros out the struct!
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX; // Setting this allows for mip maps to work! (if they exist)

	shaderResource = *_cubemap;

	// Ask DirectX for the actual object
	dev->CreateSamplerState(&samplerDesc, &samplerState);

	// Create the states for the sky
	D3D11_RASTERIZER_DESC skyRD = {};
	skyRD.CullMode = D3D11_CULL_FRONT;
	skyRD.FillMode = D3D11_FILL_SOLID;
	skyRD.DepthClipEnable = true;
	dev->CreateRasterizerState(&skyRD, _rasterState);

	D3D11_DEPTH_STENCIL_DESC skyDD = {};
	skyDD.DepthEnable = true;
	skyDD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	skyDD.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dev->CreateDepthStencilState(&skyDD, _depthState);
	///
}
