#pragma once
#include "SimpleShader.h"
#include "Texture.h"

class Material
{
public:
	Material();
	void CreateMaterial(SimpleVertexShader* vShader, SimplePixelShader* pShader, ID3D11ShaderResourceView* resource, ID3D11SamplerState* state);
	void CreateNormalMaterial(SimpleVertexShader* vShader, SimplePixelShader* pShader, ID3D11ShaderResourceView* resource, ID3D11ShaderResourceView* normal, ID3D11SamplerState* state);
	~Material();

	SimpleVertexShader* GetVertexShader();
	SimplePixelShader* GetPixelShader();

	// Getters for textring states
	ID3D11ShaderResourceView * GetShaderResourceView();
	ID3D11ShaderResourceView * GetNormalResourceView();
	ID3D11SamplerState* GetSamplerState();

private:
	SimpleVertexShader* vertexShader = nullptr;
	SimplePixelShader* pixelShader = nullptr;

	// Sampler states for texturing
	ID3D11ShaderResourceView* shaderRes = nullptr;
	ID3D11ShaderResourceView* shaderNorm = nullptr;
	ID3D11SamplerState* samplerState = nullptr;
};

