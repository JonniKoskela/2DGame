#pragma once

#include <vector>
struct attackRenderData
{
	std::vector<attackVertex> vertices{};
	int attackFlag{};
};
std::vector<ActionID> attackRenderQueue{};

bool compileShaders(BumpAllocator* bump);
bool compileArcShaders(BumpAllocator* bump);
void renderArc();
void renderSlam();
void renderAttack(ActionID);

void renderAttacks(std::vector<ActionID>& attackQueue)
{
	for (ActionID renderID : attackQueue)
	{
		renderAttack(renderID);
	}
	attackQueue.clear();

}


void renderAttack(ActionID id)
{
	switch (id)
	{
	case ARC_ATTACK:
		renderArc();
		break;

	case SLAM_ATTACK:
		renderSlam();
		break;
	}
}
