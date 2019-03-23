#pragma once
#include "Material.h"

class Emitter
{
	struct Particle 
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT4 Color;
		DirectX::XMFLOAT3 StartVel;
		float size;
		float age;
	};

	struct ParticleVertex 
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT2 UV;
		DirectX::XMFLOAT4 Color;
		float size;
	};

public:
	Emitter();
	~Emitter();

private:
	Particle* particles;
	ParticleVertex* localParticleVerticies;
	float maxAge;
	float maxSize;
};

