#pragma once
enum ActionStaticType
{
	ACTION_STATIC_EMPTY,
	ACTION_STATIC,
	ACTION_DYNAMIC
};
enum ActionID
{
	ACTIONID_EMPTY = 0,
	ARC_ATTACK = 51,
	SLAM_ATTACK = 52,
};
enum ActionType
{
	ACTION_EMPTY,
	ACTION_ATTACK,
	ACTION_SPELL,
	ACTION_ITEM,
};
class Action
{
public:
	int actionType = ACTION_EMPTY;
	int actionID = ACTIONID_EMPTY;
	float actionCoolDown{ 0.0f };
	int actionStaticType{ 0 };

	Action() = default;
};

class ActionBarSlot
{
public:
	bool active{false};
	float coolDownTimer{ 0.0f };
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