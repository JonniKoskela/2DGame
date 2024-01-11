#pragma once
#include "assets.h"
#include "renderInterface.h"
#include <chrono>
using Duration = std::chrono::duration<float>;
using Clock = std::chrono::steady_clock;

auto previousTime = Clock::now();
auto currentTime = Clock::now();
constexpr float DELTA = (float)1 / 24;
extern float deltaTime;
static Vec2 playerPos = { 0.0f, 0.0f };
extern Vec2 mPos;

void getDT();
void setupGame();


void mainGameLoop() {

	getDT();
	if (deltaTime >= DELTA)
		deltaTime -= DELTA;
	
	drawSprite(SPRITE_DOOR, Vec2{ 0.0f,0.0f });
	drawSprite(SPRITE_FROG, playerPos);

}


void setupGame() {
	playerPos = { 0.0f, 0.0f };
}

void getDT() {

	currentTime = Clock::now();

	Duration dt = currentTime - previousTime;

	previousTime = currentTime;
	deltaTime += dt.count();
}

