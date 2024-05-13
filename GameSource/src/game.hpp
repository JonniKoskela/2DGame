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
#include "action/attackTimer.hpp"
#include "GLrenderer_attackQueue.hpp"
#include "map/map.hpp"
#include "GLrenderer.h"
#include "Events.hpp"




double getRenderTime();
void checkObserverState();
void updateEquipmentPosition();
void updatePlayerMovement();
void updateKeyBindingState();



void simulate() 
{
	checkObserverState();
	updatePlayerMovement();
	updateKeyBindingState();



	//update cooldowns && process Actions
		for (ActionBarSlot& actionSlot : actionBar.actions)
		{
			if (actionSlot.onCooldown)
			{
				updateActionState(actionSlot.boundAction,false);
				actionSlot.boundAction.actionTimer.coolDownTimer += deltaTime;
				if (actionSlot.boundAction.actionTimer.coolDownTimer > actionSlot.boundAction.actionTimer.totalCoolDown)
				{
					actionSlot.boundAction.actionTimer.coolDownTimer = 0.0f;
					actionSlot.onCooldown = false;
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

	//std::cout << player.pos;
	deltaTime += getTime();
	if (deltaTime >= DELTA)
	{
		updateMousePos();
		// update current physical state of mapped keys. done by glfw
		updateKeyState();
		// calculate speed, movement, cooldowns, start attacks...
		simulate();
		deltaTime -= DELTA;
		renderTimer -= DELTA;
		//std::cout << mPos.x << " " << mPos.y << " normalized:  " << normalized.x << normalized.y<< "\n";
	}
	renderTimer += getRenderTime();
	updateActionRenderState(renderTimer);
	lerpPlayerPosition();
	updateEquipmentPosition();
	//drawSprite(SPRITE_DOOR, Vec2{ 50.0f,50.0f });

	drawSprite(SPRITE_FROG, player.renderPos);
	drawSprite(SPRITE_MOB_GOBLIN, gobo.position);

	drawPlayerEquipment(player);
	renderData->gameCamera.position = player.renderPos;

	getFPS(1.0f);
}



void initGame() 
{
	player.pos = { 50.0f, 50.0f };
	renderData->gameCamera.dimensions = { 640.0f,360.0f };
	renderData->gameCamera.position = { 0.0f, 0.0f };
	gobo = createMob(MOB_GOBLIN);
	gobo.position = { 100.0f, 100.0f };
	actionBar.actions.reserve(5);
	actionBar.actions[0].bindActionBarSlot(loadAction(ARC_ATTACK));
	actionBar.actions[0].active = true;
	actionBar.actions[1].bindActionBarSlot(loadAction(SLAM_ATTACK));
	actionBar.actions[1].active = true;
	actionBar.actions[2].bindActionBarSlot(loadAction(MOVING_ARC_ATTACK));
	actionBar.actions[2].active = true;

	player.equipment.equipWeapon(WEAPON_DAGGER_IRON);
	gameState.currentMap = MAP::Map::_initMap(MAP::MAP_START);

	LocationEventData testLED{ testBorders,testEvent };
	testLED.detachObserver = true;
	testEventList.eventList.push_back(testLED);
	static LocationEventObserver testLEO(testEventList);
	player.LEObserver = std::make_unique<LocationEventObserver>(testEventList);;
	player.LEObserver->active = true;
	//fillMapBuffer();
}

double getTime() 
{
	static auto previousTime = Clock::now();
	static auto currentTime = Clock::now();
	currentTime = Clock::now();
	Duration dt = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - previousTime);

	previousTime = currentTime;
	return dt.count();
}
double getRenderTime()
{
	static auto previousRenderTime = Clock::now();
	static auto currentRenderTime = Clock::now();
	currentRenderTime = Clock::now();
	Duration dt = std::chrono::duration_cast<std::chrono::microseconds>(currentRenderTime - previousRenderTime);

	previousRenderTime = currentRenderTime;
	return dt.count();
}

void updateMousePos()
{
	double xPos{}, yPos{};
	glfwGetCursorPos(window, &xPos, &yPos);
	OrtographicCamera camera = renderData->gameCamera;

	mPos.x = (xPos - (winWidth / 2)) / (winWidth / renderData->gameCamera.dimensions.x) + camera.position.x;
	mPos.y = -(yPos - winHeight / 2) / (winHeight / renderData->gameCamera.dimensions.y) + camera.position.y;
}


void updateActionRenderState(double renderTimer)
{
	for (ActionBarSlot& abs : actionBar.actions)
	{
		if (abs.onCooldown)
		{
			updateActionState(abs.boundAction, true);
		}
	}
}

void lerpPlayerPosition()
{
	{
		player.renderPos.x = player.pos.x + (player.speed.x*(renderTimer/DELTA));
		player.renderPos.y = player.pos.y + (player.speed.y * (renderTimer / DELTA));
	}
}

void updateEquipmentPosition()
{
	if (player.equipment.ContainsWeapon)
	{
		Sprite wepSprite = player.equipment.currentWeapon.weaponSprite;

		/*player.weaponRenderData.renderData = */
		

	}
}

void updatePlayerMovement()
{
	
	float runSpeed = 2.0f;
	float runAccel = 17.0f;
	float runReduce = 7.0f;
	float gravity = 3.0f;

	bool yKeyDown = (pollAction(MOVE_DOWN, KEY_DOWN) == true || pollAction(MOVE_UP, KEY_DOWN) == true);
	bool xKeyDown = (pollAction(MOVE_RIGHT, KEY_DOWN) == true || pollAction(MOVE_LEFT, KEY_DOWN) == true);

	if (xKeyDown)
	{
		runAccel /= 1.6f;
		runSpeed *= 0.71f;
	}
	if (yKeyDown)
	{
		runAccel /= 1.6f;
		runSpeed *= 0.71f;
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
	}
}

void updateKeyBindingState()
{
	{
		if (pollAction(ACTIONBAR_1, KEY_DOWN) == true && actionBar.actions[0].onCooldown == false)
		{
			actionBar.actions[0].onCooldown = true;
			player.weaponRenderData.playerWeaponOnLeft = !player.weaponRenderData.playerWeaponOnLeft;
		}
		if (pollAction(ACTIONBAR_2, KEY_DOWN) == true && actionBar.actions[1].onCooldown == false)
		{
			actionBar.actions[1].onCooldown = true;
			player.weaponRenderData.playerWeaponOnLeft = !player.weaponRenderData.playerWeaponOnLeft;
		}
		if (pollAction(ACTIONBAR_3, KEY_DOWN) == true && actionBar.actions[2].onCooldown == false)
		{
			actionBar.actions[2].onCooldown = true;
			player.weaponRenderData.playerWeaponOnLeft = !player.weaponRenderData.playerWeaponOnLeft;
		}
	}
}

void checkObserverState()
{ 
	if (player.LEObserver->active)
	{
		player.LEObserver->updateEventStatus();
	}
}