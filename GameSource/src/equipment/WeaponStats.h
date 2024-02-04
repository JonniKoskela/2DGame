#pragma once
#include "equipment_ENUM.h"
class WeaponStats
{
public:
	int damage{0};
	int spellDamage{0};

	WeaponStats() = default;
	WeaponStats(WeaponID id)
	{
		switch (id)
		{
		case WEAPON_DAGGER_IRON:
			damage = 5;
		}
	}
};

