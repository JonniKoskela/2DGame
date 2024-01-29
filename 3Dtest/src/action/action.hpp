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


void ActionBarSlot::startAction()
{
	if (this->boundAction.actionType == ACTION_ATTACK)
	{
		startAttack(this->boundAction.actionID);
	}
}
void ActionBarSlot::bindActionBarSlot(ActionType type, ActionID id)
{
	this->boundAction = Action{};
	this->boundAction.actionType = type;
	this->active = true;
	this->boundAction.actionID = id;
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
	action.actionType = ACTION_ATTACK;
	switch (id)
	{
	case ACTION_ATTACK:

		action.actionCoolDown = 1500.0f;
		break;
	case ACTION_ITEM:
		break;
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
	case ARC_ATTACK:
		break;
	}
}