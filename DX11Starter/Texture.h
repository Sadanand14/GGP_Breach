#pragma once
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
#include "SimpleShader.h"

class Texture
{
public:
	Texture();
	~Texture();
	// Getters for textring states
	ID3D11ShaderResourceView * GetShaderResourceView();
	ID3D11SamplerState* GetSamplerState();
	void CreateTexure(ID3D11Device* dev, ID3D11DeviceContext* devContext, const wchar_t* fileName, ID3D11ShaderResourceView** _texture);
	void CreateCubeMap(ID3D11Device* dev, ID3D11DeviceContext* devContext, const wchar_t* fileName, ID3D11ShaderResourceView** _cubemap);

private:
	//// Loading in textures
	ID3D11ShaderResourceView* shaderResource = nullptr;

	// Sampler state
	ID3D11SamplerState* samplerState = nullptr;
};

