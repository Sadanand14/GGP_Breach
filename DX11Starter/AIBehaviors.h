#pragma once

#include <vector>
#include <DirectXMath.h>
#include "Entity.h"

class AIBehaviors
{
public:
	AIBehaviors(Entity*);
	~AIBehaviors();
	Entity* agent = nullptr;

	void SetWaypoints(std::vector<DirectX::XMFLOAT3>);
	void WaypointsLerp(std::vector<DirectX::XMFLOAT3>, float);
	
private:
	std::vector<DirectX::XMFLOAT3> waypoints;
	float dt;
};