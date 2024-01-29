#pragma once
#include "assets.h"
#include "mobs.h"
#include "renderInterface.h"
#include "input.hpp"
#include <chrono>
#include "camera.h"
#include "action/action.hpp"
#include "action/attackActionStructs.h"
#include "game.h"
#include <math.h>



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
			//std::cout << player.pos.x << " " << player.pos.y << "\n";
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


		std::cout << "cd timer:" << actionBar.actions[0].coolDownTimer << "\n";
		std::cout << "delta: " << deltaTime << "\n";
		{
			if (pollAction(ACTIONBAR_1, KEY_DOWN) == true && actionBar.actions[0].onCooldown == false)
			{
				std::cout << actionBar.actions[0].coolDownTimer<< "\n";
				std::cout << actionBar.actions[0].boundAction.actionCoolDown<< "\n";
				actionBar.actions[0].startAction();
				actionBar.actions[0].onCooldown = true;
			}
			if (pollAction(ACTIONBAR_2, KEY_DOWN) == true && actionBar.actions[1].onCooldown == false)
			{
				actionBar.actions[1].coolDownTimer;
				actionBar.actions[1].startAction();
				actionBar.actions[1].onCooldown = true;
			}
		}
	}

	//update cooldowns && process Actions
		for (ActionBarSlot& actionSlot : actionBar.actions)
		{
			actionSlot.coolDownTimer += deltaTime;
			if (actionSlot.onCooldown)
			{
				if (actionSlot.coolDownTimer >= actionSlot.boundAction.actionCoolDown)
				{
					actionSlot.coolDownTimer = 0.0f;
					actionSlot.onCooldown = false;
				}
				if (actionSlot.boundAction.actionStaticType == ACTION_DYNAMIC)
				{
					processAction(actionSlot.boundAction);
				}
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
		// update current physical state of mapped keys. done by glfw
		updateKeyState();
		updateMousePos();
		// calculate speed, movement, cooldowns, start attacks...
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
	gobo.position = { 100.0f, 100.0f };
	actionBar.actions.reserve(5);
	actionBar.actions[0].bindActionBarSlot(loadAction(ARC_ATTACK));
	actionBar.actions[0].active = true;
	actionBar.actions[1].bindActionBarSlot(loadAction(SLAM_ATTACK));
	actionBar.actions[1].active = true;
}

void getDT() 
{
	currentTime = Clock::now();
	Duration dt = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousTime);

	previousTime = currentTime;
	deltaTime += dt.count();
}

void updateMousePos()
{
	double xPos{}, yPos{};
	glfwGetCursorPos(window, &xPos, &yPos);
	OrtographicCamera camera = renderData->gameCamera;

	mPos.x = (xPos - (winWidth / 2)) / (winWidth / renderData->gameCamera.dimensions.x) + camera.position.x;
	mPos.y = -(yPos - winHeight / 2) / (winHeight / renderData->gameCamera.dimensions.y) + camera.position.y;
}