#pragma once
#include "../utils.h"
#include <chrono>
constexpr float fPi = 3.141592653589793;


using Duration = std::chrono::duration<float>;
using Clock = std::chrono::steady_clock;
auto previousArcTime = Clock::now();
auto currentArcTime = Clock::now();

int arcVertexCapacity{ 25 };
bool attacking = false;

