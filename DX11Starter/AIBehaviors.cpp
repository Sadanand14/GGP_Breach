#include <vector>
#include "AIBehaviors.h"

using namespace DirectX;

AIBehaviors::AIBehaviors(Entity* ai) 
{
	this->agent = ai;
}

AIBehaviors::~AIBehaviors()
{
	// delete this->agent;
}

void AIBehaviors::SetWaypoints(std::vector<XMFLOAT3> pts)
{
	this->waypoints = pts;
}

void AIBehaviors::WaypointsLerp(std::vector<XMFLOAT3> pts, float t) {

	int size = pts.size();
	// agent->SetPosition(0, 0, 5);
	for (int i = 0; i < size - 1; i++) {
		// XMMATRIX trans = XMMatrixTranslation(agent->GetPosition().x + 1, agent->GetPosition().y, agent->GetPosition.z);
		// float tX = agent->GetPosition().x + 1;
		agent->SetPositionF(-1.5f + 0.3 * t, 0, 5);
	}
}