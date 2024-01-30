#pragma once
#include "attackTimer.h"
#include "action.h"
AttackTimer loadTimerProperties(Action& action)
{
	AttackTimer attackTimer{};
	assert((action.actionType != ACTION_EMPTY) && "error at void AttackTimer::loadTimerProperties(Action action): actionID empty");
	assert((action.actionStaticType != ACTION_STATIC_EMPTY) && "error at void AttackTimer::loadTimerProperties(Action action): actionTypeStatic is empty");
	
	if(action.actionStaticType == ACTION_STATIC)
	{
		switch (action.actionID)
		{
		case ARC_ATTACK:
			attackTimer.dynamic_attackMaxTime = 0.0f;
			attackTimer.renderTime = 1.0f;
			attackTimer.fadeTime = 1.0f;
			break;
		
		}
		return attackTimer;
	}
	//DYNAMIC
	else if(action.actionStaticType == ACTION_DYNAMIC)
	{
		switch (action.actionID)
		{
		case SLAM_ATTACK:
		{
			attackTimer.dynamic_attackMaxTime = 2.0f;
			attackTimer.renderTime = 1.0f;
			attackTimer.fadeTime = 1.0f;
		}
		}
		return attackTimer;
	}
	
}