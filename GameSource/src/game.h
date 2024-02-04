#pragma once
#include <chrono>
#include "input.h"
#include "action/action.h"
#include "mobs.h"
#include "player.h"
using Duration = std::chrono::duration<float>;
using Clock = std::chrono::steady_clock;



constexpr double DELTA = (float)1 / 60;
static Mob gobo;
static ActionBar actionBar{};


static double deltaTime = 0.0;
static double renderTimer{ 0 };

double getTime();
void setupGame();
Vec2 toWorldCoordinates(Vec2 position);
void updateMousePos();
void updateActionRenderState(double renderTimer);

void lerpPlayerPosition();
void checkActionRenderStatus(ActionBar&);
//void startArcAttack();
bool arcHitDetection(float AttackAngle);

struct GameState
{
	KeyMapping keyMappings[GAME_INPUT_COUNT];

};
static GameState gameState{};