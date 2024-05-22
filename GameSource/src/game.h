#pragma once
#include <chrono>
#include "input.h"
#include "action/action.h"
#include "mobs.h"
#include "player.h"
#include "map/map.hpp"
using Duration = std::chrono::duration<float>;
using Clock = std::chrono::steady_clock;



constexpr double DELTA = 1.0 / 60.0;
static Mob gobo;
static ActionBar actionBar{};


static double deltaTime = 0.0;
static double renderTimer{ 0 };

double getTime();
void initGame();
Vec2 toWorldCoordinates(Vec2 position);
void updateMousePos();
void updateActionRenderState(double renderTimer);

void lerpPlayerPosition();
void checkActionRenderStatus(ActionBar&);
//void startArcAttack();
bool arcHitDetection(float AttackAngle);

enum ACTIVE_GAMESTATE
{
	GAMESTATE_PLAYING,
	GAMESTATE_MAINMENU,
	GAMESTATE_PAUSED,
};


struct GameState
{
	
	KeyMapping keyMappings[GAME_INPUT_COUNT];
	MAP::Map currentMap{};
	ACTIVE_GAMESTATE activeState = GAMESTATE_PLAYING;
};

static GameState gameState{};