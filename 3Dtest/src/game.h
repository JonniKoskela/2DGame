#pragma once
#include "input.h"
#include "action/action.h"

#include <chrono>
#include "mobs.h"
using Duration = std::chrono::duration<float>;
using Clock = std::chrono::steady_clock;

auto previousTime = Clock::now();
auto currentTime = Clock::now();
constexpr float DELTA = (float)1 / 60;
static Mob gobo;
ActionBar actionBar{};


static float deltaTime = 0.0f;

void getDT();
void setupGame();
Vec2 toWorldCoordinates(Vec2 position);

void startArcAttack();
bool arcHitDetection(float AttackAngle);

struct GameState
{
	KeyMapping keyMappings[GAME_INPUT_COUNT];

};

class Player
{
public:
	Vec2 pos{};
	Vec2 speed{};
	bool attacking { false };
	bool moving { false };
};
static Player player{};
static GameState gameState{};