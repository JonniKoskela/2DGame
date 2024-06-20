#pragma once
#include "ACTION_ENUM.H"
#include "../utils.h"

struct AnimationData
{
	int frameCount{ 0 };
	iVec2 animation_atlasPosition{};
	iVec2 animation_atlasSize{};
};