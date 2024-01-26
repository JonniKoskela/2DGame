#pragma once
#include "game.h"
#include "assets.h"
#include "mobs.h"
#include "renderInterface.h"
#include "input.hpp"
#include <chrono>
#include "GLrenderer.h"
#include "camera.h"
#include "attacks.hpp"
#include <math.h>

using Duration = std::chrono::duration<float>;
using Clock = std::chrono::steady_clock;

auto previousTime = Clock::now();
auto currentTime = Clock::now();
constexpr float DELTA = (float)1 / 60;
extern float deltaTime;
Mob gobo;
ActionBar actionBar{};

void getDT();
void setupGame();
Vec2 toWorldCoordinates(Vec2 position);

void startArcAttack();
bool arcHitDetection(float AttackAngle);


void simulate() 
{
	float runSpeed = 1.5f;
	float runAccel = 15.0f;
	float runReduce = 7.0f;
	float gravity = 3.0f;
	bool yKeyDown = (pollAction(MOVE_DOWN, KEY_DOWN) == true || pollAction(MOVE_UP, KEY_DOWN) == true);
	bool xKeyDown = (pollAction(MOVE_RIGHT, KEY_DOWN) == true || pollAction(MOVE_LEFT, KEY_DOWN) == true);

	if (xKeyDown)
	{
		runAccel /= 1.7f;
	}
	if (yKeyDown)
	{
		runAccel /= 1.7f;
	}

	//calculate new speed for player
	{
		if (yKeyDown)
		{
			std::cout << player.pos.x << " " << player.pos.y << "\n";
			if (pollAction(MOVE_UP, KEY_DOWN) == true)
			{
				player.speed.y = approach(player.speed.y, runSpeed, runAccel * DELTA);
			}
			if (pollAction(MOVE_DOWN, KEY_DOWN) == true)
			{
				player.speed.y = approach(player.speed.y, -runSpeed, runAccel * DELTA);
			}
		}
		else
		{
			player.speed.y = approach(player.speed.y, 0.0f, runReduce * DELTA);
		}
		if (xKeyDown)
		{
			if (pollAction(MOVE_RIGHT, KEY_DOWN) == true)
			{
				player.speed.x = approach(player.speed.x, runSpeed, runAccel * DELTA);
			}
			if (pollAction(MOVE_LEFT, KEY_DOWN) == true)
			{
				player.speed.x = approach(player.speed.x, -runSpeed, runAccel * DELTA);
			}
		}
		else 
		{
			player.speed.x = approach(player.speed.x, 0.0f, runReduce * DELTA);
		}

		if (pollAction(ATTACK_1,KEY_DOWN) == true && attacking != true)
		{
			actionBar.startAttack( actionBar.actions[0] );
		}
		else if (pollAction(ATTACK_2, KEY_DOWN) == true && attacking != true)
		{
			actionBar.startAttack( actionBar.actions[1] );
		}
	}

	//calculate new position if changed
	{
		player.pos.x += player.speed.x;
		player.pos.y += player.speed.y;
	}
}

void mainGameLoop() 
{
	getDT();
	if (deltaTime >= DELTA)
	{
		updateKeyState();
		simulate();
		deltaTime -= DELTA;
		//std::cout << mPos.x << " " << mPos.y << " normalized:  " << normalized.x << normalized.y<< "\n";
	}

	//drawSprite(SPRITE_DOOR, Vec2{ 50.0f,50.0f });
	drawSprite(SPRITE_FROG, player.pos);
	drawSprite(SPRITE_MOB_GOBLIN, gobo.position);
	renderData->gameCamera.position = player.pos;
}


void setupGame() 
{
	player.pos = { 50.0f, 50.0f };
	renderData->gameCamera.dimensions = { 480.0f,270.0f };
	renderData->gameCamera.position = { 0.0f, 0.0f };
	gobo = createMob(MOB_GOBLIN);
	gobo.position = { 100.0f,100.0f };
	actionBar.actions.reserve(5);
	actionBar.actions[0] = ARC_ATTACK;
	actionBar.actions[1] = SLAM_ATTACK;
}

void getDT() 
{
	currentTime = Clock::now();
	Duration dt = currentTime - previousTime;

	previousTime = currentTime;
	deltaTime += dt.count();
}

