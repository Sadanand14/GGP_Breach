#include "Material.h"

Material::Material()
{
}

void Material::CreateMaterial(SimpleVertexShader * vShader, SimplePixelShader * pShader, ID3D11ShaderResourceView * resource, ID3D11SamplerState * state)
{
	vertexShader = vShader;
	pixelShader = pShader;
	shaderRes = resource;
	samplerState = state;
	}

void Material::CreateNormalMaterial(SimpleVertexShader * vShader, SimplePixelShader * pShader, ID3D11ShaderResourceView * resource, ID3D11ShaderResourceView * normal, ID3D11SamplerState * state)
{
	vertexShader = vShader;
	pixelShader = pShader;
	shaderRes = resource;
	shaderNorm = normal;
	samplerState = state;
}

Material::~Material()
{
}

SimpleVertexShader * Material::GetVertexShader()
{
	return vertexShader;
}

SimplePixelShader * Material::GetPixelShader()
{
	return pixelShader;
}

ID3D11ShaderResourceView * Material::GetShaderResourceView()
{
	return shaderRes;
}

ID3D11ShaderResourceView * Material::GetNormalResourceView()
{
	return shaderNorm;
}

ID3D11SamplerState * Material::GetSamplerState()
{
	return samplerState;
}
