#pragma once
#include <iostream>
#include <chrono>
#include "action.h"
#include "attackTimer.hpp"
#include "attackProperties.hpp"
#include "actionDrawData.hpp"


Action loadAction(ActionID id);
void startAttack(int actionID);
//void startArcAttack();
//void startSlamAttack();
void processAttack_dynamic(Action& action, bool renderer);
void processAttack_static(Action& action, bool renderer);
void processAttack(Action& action, bool renderer);
void processSlam(Action& action, bool renderer);
void processArc(Action& action, bool renderer);
void processMovingArc(Action& action,Player&, bool renderer);


void ActionBarSlot::bindActionBarSlot(Action action)
{
	this->boundAction = action;
	this->boundAction.actionType = action.actionType;
	this->boundAction.actionStaticType = action.actionStaticType;
	this->active = true;
	this->boundAction.actionID = action.actionID;
	this->boundAction.actionTimer = loadAttackTimer(action);
}

//void ActionBarSlot::startAction()
//{
//	if (this->boundAction.actionType == ACTION_ATTACK)
//	{
//		startAttack(this->boundAction.actionID);
//	}
//}

//void startAttack(int actionID)
//{
//	switch (actionID)
//	{
//	case ARC_ATTACK:
//		startArcAttack();
//		break;
//	case SLAM_ATTACK:
//		startSlamAttack();
//		break;
//	}
//}

Action loadAction(ActionID id)
{
	Action a{};

	a.drawData = loadDrawData(id);
	if(id >= 50)
	{
		a.attackProperties = loadAttackProperties(id);
		a.actionType = ACTION_ATTACK;
		switch (id)
		{
		case ARC_ATTACK:
			a.actionStaticType = ACTION_STATIC;
			a.actionID = id;
			break;

		case SLAM_ATTACK:
			a.actionStaticType = ACTION_DYNAMIC;
			a.actionID = id;
			break;
		case MOVING_ARC_ATTACK:
			a.actionStaticType = ACTION_STATIC;
			a.actionID = id;
		}
	}
	std::cout << "loaded attack" << "\n";
	return a;

	if (id < 50)
	{

	}
}

void updateActionState(Action& action, bool renderer)
{
	switch (action.actionType)
	{
	case ACTION_ATTACK:
		if (action.actionStaticType == ACTION_STATIC)
		{
			processAttack_static(action, renderer);
		}
		if (action.actionStaticType == ACTION_DYNAMIC)
		{
			processAttack_dynamic(action, renderer);
		}
	}
}

//void processAttack(Action& action, bool renderer)
//{
//	switch (action.actionID)
//	{
//	case SLAM_ATTACK:
//		processSlam(action, bool renderer);
//		break;
//	case ARC_ATTACK:
//		processArc(action, bool renderer);
//	}
//}

void processAttack_static(Action& action, bool renderer)
{
	switch (action.actionID)
	{
	case ARC_ATTACK:
		processArc(action, renderer);
		break;

	case MOVING_ARC_ATTACK:
		processMovingArc(action,player,renderer);
		break;
	}

}
void processAttack_dynamic(Action& action, bool renderer)
{
	switch (action.actionID)
	{
	case SLAM_ATTACK:
		processSlam(action, renderer);
		break;
	}
}