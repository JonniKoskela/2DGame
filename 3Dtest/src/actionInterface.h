#pragma once

#include <iostream>
#include <chrono>
constexpr float fPi = 3.141592653589793;


using Duration = std::chrono::duration<float>;
using Clock = std::chrono::steady_clock;
auto previousArcTime = Clock::now();
auto currentArcTime = Clock::now();

int arcVertexCapacity{ 25 };
float arcTimer{ 0.0f };
float slamTimer{ 0.0f };
float attackFadeTime = 1500.0f;
bool attacking = false;

void resetTimer();
bool arcHitDetection(float AttackAngle);




struct ArcVertex
{
	Vec2 vertex;
};
std::vector<ArcVertex> generateArcVertices(Vec2& pos, float mAngle, float distance);
void genArcBuffer(std::vector<ArcVertex> arcVertices);
void startArcAttack();

struct SlamVertex
{
	Vec2 vertex;
};
std::vector<SlamVertex> generateSlamVertices(Vec2& pos, float mAngle, float range);
void genSlamBuffer(std::vector<SlamVertex> slamVertices);
void startSlamAttack();


enum ActionStaticType
{
	ATTACK_STATIC,
	ATTACK_DYNAMIC
};
enum AttackID
{
	ATTACK_EMPTY,
	ARC_ATTACK,
	SLAM_ATTACK,
};
enum ActionType
{
	ACTION_EMPTY,
	ACTION_ATTACK,
	ACTION_SPELL,
	ACTION_ITEM,
};
class ActionBarSlot
{
public:
	bool active{false};
	ActionType type = ACTION_EMPTY;
	ActionStaticType actionStaticType{};
	void bindActionSlot(ActionType actionType,int actionID);
	int attackID = 0;
};
class Action
{
public:
	float actionCoolDown{};

};
class ItemEquip
{

};
class ItemConsumable
{

};


void ActionBarSlot::bindActionSlot(ActionType actionType, int actionID)
{
	
	if(actionType == ACTION_ATTACK){
		switch (actionID)
		{
		case ARC_ATTACK:
			this->type = ACTION_ATTACK;
			this->attackID = ARC_ATTACK;
			break;

		case SLAM_ATTACK:
			this->attackID = SLAM_ATTACK;
			break;
		}
	}
}

class ActionBar
{
public:
	std::vector<ActionBarSlot> actions{5};
	void startAction(ActionBarSlot& action);
};

void ActionBar::startAction(ActionBarSlot& action)
{
	if(action.type == ACTION_ATTACK){
		switch (action.attackID)
		{
		case ARC_ATTACK:
			startArcAttack();
			break;

		case SLAM_ATTACK:
			startSlamAttack();
			break;
		}
	}
}
