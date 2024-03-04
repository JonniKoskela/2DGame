#pragma once

#include <vector>

std::vector<ActionID> attackRenderQueue{};




void drawAttack(RenderData4xVec2* data)
{
	attackTransforms.push_back(*data);

}