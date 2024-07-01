#pragma once
#include "game.h"
#include "targeting.hpp"
#include "assets.h"
#include "renderInterface.h"
#include "input.hpp"
#include <chrono>
#include "camera.h"
#include "action/action.hpp"
#include "action/attackActionStructs.h"

#include "action/attackTimer.hpp"
#include "GLrenderer_attackQueue.hpp"
#include "map/map.hpp"
#include "GLrenderer.h"
#include "Events.hpp"


constexpr int GAME_TILESIZE = 32;


double getRenderTime();
void checkObserverState();
void updateEquipmentPosition();
void updatePlayerMovement();
void updateKeyBindingState();
bool checkCollision();


void simulate() 
{
	checkObserverState();
	updatePlayerMovement();
	updateKeyBindingState();

	if (pollAction(TAB_TARGET, KEY_DOWN))
	{
	    player.target.tabTarget();
	}


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


}

void mainGameLoop() 
{

	//std::cout << player.pos;
	deltaTime += getTime();

	if (deltaTime >= DELTA)
	{
		updateMousePos();
		//static iVec2 mp{ (int)mPos.x / 32, (int)mPos.y / 32 };
		//std::cout << mp << "\n";
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
	
	drawSprite(SPRITE_FROG, player.renderPos);
	drawMobs();

	if(player.currentTarget){
	    drawDebugQuad(player.currentTarget->position, 6);
	}

	//drawPlayerEquipment(player);
	renderData->gameCamera.position = player.renderPos;

	getFPS(1.0f);
}



void initGame() 
{
	player.pos = {50,50};
	renderData->gameCamera.dimensions = { 640.0f,360.0f };
	renderData->gameCamera.position = { 0.0f, 0.0f };
	Vec2 range = { 50.0f,200.0f };
	gameState.mobs.mobList.push_back(createMob(MOB_GOBLIN, randomVec2f(range)));
	gameState.mobs.mobList.push_back(createMob(MOB_GOBLIN, randomVec2f(range)));
	gameState.mobs.mobList.push_back(createMob(MOB_GOBLIN, randomVec2f(range)));
	gameState.mobs.mobList.push_back(createMob(MOB_GOBLIN, randomVec2f(range)));
	gameState.mobs.mobList.push_back(createMob(MOB_GOBLIN, randomVec2f(range)));
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
}

inline double getTime() 
{
	static auto previousTime = Clock::now();
	static auto currentTime = Clock::now();
	currentTime = Clock::now();
	Duration dt = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - previousTime);

	previousTime = currentTime;
	return dt.count();
}
inline double getRenderTime()
{
	static auto previousRenderTime = Clock::now();
	static auto currentRenderTime = Clock::now();
	currentRenderTime = Clock::now();
	Duration dt = std::chrono::duration_cast<std::chrono::microseconds>(currentRenderTime - previousRenderTime);

	previousRenderTime = currentRenderTime;
	return dt.count();
}

inline void updateMousePos()
{
	double xPos{}, yPos{};
	glfwGetCursorPos(window, &xPos, &yPos);
	OrtographicCamera camera = renderData->gameCamera;

	mPos.x = (xPos - (winWidth / 2)) / (winWidth / renderData->gameCamera.dimensions.x) + camera.position.x;
	mPos.y = -(yPos - winHeight / 2) / (winHeight / renderData->gameCamera.dimensions.y) + camera.position.y;
}


inline void updateActionRenderState(double renderTimer)
{
	for (ActionBarSlot& abs : actionBar.actions)
	{
		if (abs.onCooldown)
		{
			updateActionState(abs.boundAction, true);
		}
	}
}

inline void lerpPlayerPosition()
{
	{
		player.renderPos.x = player.pos.x + (player.speed.x * (renderTimer / DELTA));
		player.renderPos.y = player.pos.y + (player.speed.y * (renderTimer / DELTA));
	}
}

inline void updateEquipmentPosition()
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
	checkCollision();
}

inline void checkObserverState()
{ 
	if (player.LEObserver->active)
	{
		player.LEObserver->updateEventStatus();
	}
}
bool checkCollision()
{
	constexpr iVec2 tileSize{ 32,32 };
	Vec2 futurePosition = player.pos + player.speed;
	iRect playerRect = getPlayerRect(player);


	if (player.speed.x != 0)
	{
		auto moveX = [](Vec2 futurePosition, iRect playerRect)
			{
				int xi = futurePosition.x / 32;
				int yi = futurePosition.y / 32;
				auto grid = gameState.currentMap.gameGrid;
				auto width = gameState.currentMap.map_width;
				iVec2 playerGridPos = {player.pos.x / GAME_TILESIZE, player.pos.y / GAME_TILESIZE };

				for (int x = playerGridPos.x - 1; x <= playerGridPos.x + 1; ++x)
				{
					for (int y = playerGridPos.y - 1; y <= playerGridPos.y + 1; ++y)
					{
						if (!grid[x][y])
						{
							continue;
						}
						iVec2 v = { x * 32,y * 32 };
						iRect tileRect = { v,tileSize };
						if (rectCollision(playerRect,tileRect))
						{
							std::cout << "\n playerRect :" << playerRect.pos << " collided with tile " << tileRect.pos << x << y << "\n";
							player.pos.x -= sign(player.speed.x);
							player.speed.x = 0;

							return;
						}
					}
				}
				player.pos.x += player.speed.x;
			};
		moveX(futurePosition, playerRect);
	}
	if (player.speed.y != 0)
	{
		auto moveY = [](Vec2 futurePosition, iRect playerRect)
			{
				int xi = futurePosition.x / 32;
				int yi = futurePosition.y / 32;
				auto grid = gameState.currentMap.gameGrid;
				auto width = gameState.currentMap.map_width;
				iVec2 playerGridPos = { player.pos.x / GAME_TILESIZE, player.pos.y / GAME_TILESIZE };

				for (int x = playerGridPos.x - 1; x <= playerGridPos.x + 1; ++x)
				{
					for (int y = playerGridPos.y - 1; y <= playerGridPos.y + 1; ++y)
					{
						if (grid[x][y] == false)
						{
							continue;
						}
						iVec2 v = { x * 32,y * 32 };
						iRect tileRect = { v,tileSize };
						if (rectCollision(playerRect, tileRect))
						{
							player.speed.y = 0;
							player.pos.y-= sign(player.speed.y);
							return;
						}
					}
				}
				player.pos.y += player.speed.y;
			};
		moveY(futurePosition, playerRect);
	}
	return false;
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

//bool checkCollision()
//{
//	constexpr iVec2 tileSize{ 32,32 };
//	static Vec2 remainder;
//
//	Vec2 futurePosition = player.pos + player.speed;
//	Vec2 size = vec_2(player.size);
//	Rect playerTile{futurePosition, size};
//
//	
//	if (player.speed.x != 0)
//	{
//		auto moveX = [](Vec2 futurePosition, Rect playerTile)
//			{
//				int xi = futurePosition.x / 32;
//				int yi = futurePosition.y / 32;
//				auto coll = gameState.currentMap.collisionLayer;
//				auto width = gameState.currentMap.map_width;
//				//iRect destinationTile = {iVec2{xi,yi}, tileSize};
//				int index = yi * width + xi;
//				for (int i = -1; i <= 1; ++i)
//				{
//
//					if (coll.test(index + width * i))
//					{
//						iVec2 tilePos{ xi * 32,(yi + i) * 32 };
//						iRect tile = { tilePos,tileSize };
//						if (rectCollision(playerTile, tile))
//						{
//							//std::cout << "checking tile:" << tile.pos << "\n";
//							player.speed.x = 0;
//							std::cout << "collided with tile: " << tile.pos << " " << index << "\n";
//							return true;
//						}
//					}
//				}
//			};
//		moveX(futurePosition, playerTile);
//	}
//		if(player.speed.y != 0)
//		{
//			auto moveY = [](Vec2 futurePosition, Rect playerTile)
//				{
//					int xi = futurePosition.x / 32;
//					int yi = futurePosition.y / 32;
//					auto coll = gameState.currentMap.collisionLayer;
//					auto width = gameState.currentMap.map_width;
//					//iRect destinationTile = {iVec2{xi,yi}, tileSize};
//					int index = yi * width + xi;
//					for (int i = -1; i <= 1; ++i)
//					{
//
//						if (coll.test(index + width * i))
//						{
//							iVec2 tilePos{ (xi+i) * 32, yi * 32 };
//							iRect tile = { tilePos,tileSize };
//							if (rectCollision(playerTile, tile))
//							{
//								player.speed.y = 0;
//								std::cout << "collided with tile: " << tile.pos  << " " <<index << "\n";
//								return true;
//							}
//						}
//					}
//				};
//			moveY(futurePosition, playerTile);
//		}
//	
//
//
//	player.pos.x += player.speed.x;
//	player.pos.y += player.speed.y;
//
//
//	return false;
//}