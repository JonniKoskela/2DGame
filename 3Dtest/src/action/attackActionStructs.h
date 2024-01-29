#pragma once
#include "../utils.h"
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

struct ArcVertex
{
	Vec2 vertex;
};

struct SlamVertex
{
	Vec2 vertex;
};
