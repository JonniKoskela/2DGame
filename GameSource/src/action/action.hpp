#pragma once
#include <iostream>
#include <chrono>
#include "action.h"


void resetTimer();

Action loadAction(ActionID id);
void startAttack(int actionID);
void startArcAttack();
void startSlamAttack();
void processAttack(Action& action);
void processArc();
void processSlam();


void ActionBarSlot::startAction()
{
	if (this->boundAction.actionType == ACTION_ATTACK)
	{
		startAttack(this->boundAction.actionID);
	}
}
void ActionBarSlot::bindActionBarSlot(Action action)
{
	this->boundAction = action;
	this->boundAction.actionType = action.actionType;
	this->boundAction.actionStaticType = action.actionStaticType;
	this->active = true;
	this->boundAction.actionID = action.actionID;

}

void startAttack(int actionID)
{
	switch (actionID)
	{
	case ARC_ATTACK:
		startArcAttack();
		break;
	case SLAM_ATTACK:
		startSlamAttack();
		break;
	}
}

Action loadAction(ActionID id)
{
	Action action{};
	if(id > 50)
	{
		action.actionType = ACTION_ATTACK;
		switch (id)
		{
		case ARC_ATTACK:
			action.actionCoolDown = 1.5f;
			action.actionStaticType = ACTION_STATIC;
			action.actionID = id;
			break;
		case SLAM_ATTACK:
			action.actionCoolDown = 3.0f;
			action.actionStaticType = ACTION_DYNAMIC;
			action.actionID = id;
			break;
		}
	}
	std::cout << "loaded attack" << "\n";
	return action;
}

void processAction(Action& action)
{
	switch (action.actionType)
	{
	case ACTION_ATTACK:
		processAttack(action);
		break;
	}
}

void processAttack(Action& action)
{
	switch (action.actionType)
	{
	case SLAM_ATTACK:
		processSlam();
		break;
	}
}