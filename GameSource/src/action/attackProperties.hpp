#pragma once
#include "attackProperties.h"
AttackProperties* loadAttackProperties(ActionID id)
{
	AttackProperties* attackProperties = new AttackProperties;
	assert((id >= 50) && "error at: loadAttackProperties;\n attackProperties.hpp; \n ActionID id does not correspond to attack type");

	switch (id)
	{
	case ARC_ATTACK:

		break;

	case MOVING_ARC_ATTACK:
	{
		attackProperties->range = 240.0f;
		attackProperties->width = 80.0f;
		break;
	}
	case SLAM_ATTACK:
	{
		break;
	}

	}
		return attackProperties;

}