#pragma once
#include "WICTextureLoader.h"

class Texture
{
public:
	Texture(ID3D11Device* dev, ID3D11DeviceContext* devContext, const wchar_t* fileName, ID3D11ShaderResourceView** texture);
	~Texture();
	// Getters for textring states
	ID3D11ShaderResourceView * GetShaderResourceView();
	ID3D11SamplerState* GetSamplerState();

private:
	//// Loading in textures
	ID3D11ShaderResourceView* shaderResource = nullptr;

	// Sampler state
	ID3D11SamplerState* samplerState = nullptr;
};

