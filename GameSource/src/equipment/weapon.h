#pragma once
#include "WeaponStats.h"
#include "../assets.h"
#include "equipment_ENUM.h"

//class EquipmentSet;


class Weapon
{

public:
	WeaponID weaponID{};
	Sprite weaponSprite{};
	WeaponStats weaponStats;
	Weapon() = default;
	Weapon(WeaponID id)
	{
		weaponID = id;
		weaponSprite = getWeaponSprite(id);
		weaponStats = WeaponStats(id);
	}
private:
	Sprite getWeaponSprite(WeaponID);
};


Sprite Weapon::getWeaponSprite(WeaponID id)
{
	Sprite sprite{};
	switch (id)
	{
	case WEAPON_DAGGER_IRON:
		sprite.atlasOffset = { 0,32 };
		sprite.size = { 5,16 };

		/*case WEAPON_SWORD_IRON:
			...*/
	}
	return sprite;
}
