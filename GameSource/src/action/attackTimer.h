#pragma once
#include "../utils.h"
#include "ACTION_ENUM.H"
#include <assert.h>

//AttackTimer 
class AttackTimer
{
public:
	float renderTime{};
	float fadeTime{};
	float dynamic_attackMaxTime{};
	float totalCoolDown{};
	float coolDownTimer{};
};