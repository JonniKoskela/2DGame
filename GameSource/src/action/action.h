#pragma once
#include "ACTION_ENUM.H"
#include "attackTimer.h"
class Action
{
public:
	int actionType = ACTION_EMPTY;
	int actionID = ACTIONID_EMPTY;
	float actionCoolDown{ 0.0f };
	float coolDownTimer{};
	int actionStaticType{ 0 };
	AttackTimer attackTimer{};

	Action() = default;
};

class ActionBarSlot
{
public:
	bool active{false};
	bool onCooldown{ false };
	Action boundAction{};
	void bindActionBarSlot(Action);
	void startAction();
	

	ActionBarSlot() = default;
};

class ActionBar
{
public:
	std::vector<ActionBarSlot> actions{ 5 };

	ActionBar() = default;
};