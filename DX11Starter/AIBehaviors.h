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
	// void WaypointsLerp(std::vector<DirectX::XMFLOAT3>);
	void WaypointsLerp(DirectX::XMFLOAT3, DirectX::XMFLOAT3);
	float lerp(float, float, float);

	bool initLerp;
	int lerpPeriods;

private:
	std::vector<DirectX::XMFLOAT3> waypoints;
	float t;

	bool innerLerp;
	bool lerping;
	float speed;
};