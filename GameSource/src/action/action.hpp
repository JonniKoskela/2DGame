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
void processSlam(Action& action);


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
	this->boundAction.attackTimer = loadTimerProperties(action);
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
	Action a{};
	if(id > 50)
	{
		a.actionType = ACTION_ATTACK;
		switch (id)
		{
		case ARC_ATTACK:
			a.actionCoolDown = 1.5f;
			a.actionStaticType = ACTION_STATIC;
			a.actionID = id;
			break;
		case SLAM_ATTACK:
			a.actionCoolDown = 3.0f;
			a.actionStaticType = ACTION_DYNAMIC;
			a.actionID = id;
			break;
		}
	}
	std::cout << "loaded attack" << "\n";
	return a;
}

void processDynamic(Action& action)
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
	switch (action.actionID)
	{
	case SLAM_ATTACK:
		processSlam(action);
		break;
	}
}