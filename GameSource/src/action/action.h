#pragma once
#include "ACTION_ENUM.H"
#include "attackTimer.h"
#include "../GLRenderer_attackRenderData.h"
#include "attackProperties.hpp"
#include "actionDrawData.h"
class Action
{
public:
	int inPhase{ 0 };
	int actionType = ACTION_EMPTY;
	ActionID actionID = ACTIONID_EMPTY;

	int actionStaticType{ 0 };
	
	AnimationData drawData;
	ActionTimer actionTimer{};
	AttackProperties attackProperties;
	RenderData4xVec2* currentVertices = nullptr;
	Action() = default;
	~Action()
	{
		std::cout << "Action Deleted" << "\n";
	}
};

class ActionBarSlot
{
public:
	bool active{false};
	bool onCooldown{ false };
	Action boundAction{};
	void bindActionBarSlot(Action);

	ActionBarSlot() = default;
};

class ActionBar
{
public:
	std::vector<ActionBarSlot> actions{ 5 };

	ActionBar() = default;
};