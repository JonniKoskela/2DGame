#pragma once

#include <vector>

std::vector<ActionID> attackRenderQueue{};

bool compileShaders(BumpAllocator* bump);
bool compileArcShaders(BumpAllocator* bump);
void renderArc();
void renderSlam();


void drawAttack(attackRenderData4xVec2* data)
{
	attackTransforms.push_back(*data);

}