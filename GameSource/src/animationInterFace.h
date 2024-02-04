#pragma once
#include "action/action.h"
#include "GLRenderer_attackRenderData.h"

int animate(float timer,int frameCount, float duration)
{
	int animationIdx = 0;

	assert((duration != 0) && "error at animationInterFace.h \n int animate(...)\n duration cannot be 0");
	assert((frameCount != 0) && "error at animationInterFace.h \n int animate(...)\n frameCount cannot be 0");
	animationIdx = (int)((timer / duration) * frameCount);

	if (animationIdx >= frameCount)
	{
		animationIdx = frameCount - 1;
	}
	return animationIdx;
};

void push_animationFrame(int frameIndex, iVec2& atlasPosition, iVec2& atlasSize,attackRenderData4xVec2& animContainer)
{
	
	animContainer.attackFlag = 1;
	animContainer.textureAtlasPosition.x = atlasPosition.x + (atlasSize.x * frameIndex);
	animContainer.textureAtlasPosition.y = atlasPosition.y;
	animContainer.textureAtlasOffset = atlasSize;


	attackTransforms.push_back(animContainer);
}