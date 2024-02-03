#pragma once
#include "ACTION_ENUM.H"
#include "attackTimer.h"
#include "../GLRenderer_attackRenderData.h"
class Action
{
public:
	int inPhase{ 0 };
	int actionType = ACTION_EMPTY;
	ActionID actionID = ACTIONID_EMPTY;
	int actionStaticType{ 0 };
	AttackTimer attackTimer{};
	attackRenderData4xVec2* currentVertices = nullptr;
	Action() = default;
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