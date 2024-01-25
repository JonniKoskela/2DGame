#pragma once
#include "input.h"
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
static GameState gameState{};