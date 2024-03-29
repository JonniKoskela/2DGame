#pragma once
#include "../utils.h"
#include "ACTION_ENUM.H"
#include <assert.h>

//AttackTimer 
class ActionTimer
{
public:
	float renderTime{};
	float fadeTime{};
	float dynamic_attackMaxTime{};
	float totalCoolDown{};
	float coolDownTimer{};
	float activeTime{};
	float backSwingTime{};
};