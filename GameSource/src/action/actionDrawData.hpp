#pragma once
#include "actionDrawData.h"


DrawData loadDrawData(ActionID id)
{
	DrawData dd{};

	switch (id)
	{
	case MOVING_ARC_ATTACK:
		dd.frameCount = 5;
		dd.animation_atlasPosition = { 0,992 };
		dd.animation_atlasSize = { 16,32 };
		break;
	}

	return dd;
}