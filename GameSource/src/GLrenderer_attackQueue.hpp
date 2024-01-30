#pragma once

#include <vector>
struct attackRenderData
{
	std::vector<attackVertex> vertices{};
	int attackFlag{};
};
enum AttackRenderID
{
	RENDER_ARC,
	RENDER_SLAM
};
std::vector<AttackRenderID> attackRenderQueue{};

bool compileShaders(BumpAllocator* bump);
bool compileArcShaders(BumpAllocator* bump);
void renderArc();
void renderSlam();
void renderAttack(AttackRenderID);

void renderAttacks(std::vector<AttackRenderID>& attackQueue)
{
	for (AttackRenderID renderID : attackQueue)
	{
		renderAttack(renderID);
	}
	attackQueue.clear();

}


void renderAttack(AttackRenderID id)
{
	switch (id)
	{
	case RENDER_ARC:
		renderArc();
		break;

	case RENDER_SLAM:
		renderSlam();
		break;
	}
}
