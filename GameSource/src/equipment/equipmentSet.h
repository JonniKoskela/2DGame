#pragma once
#include "weapon.h"
class Player;
void weaponChanged();
class EquipmentSet
{
public:
	bool ContainsWeapon{false};
	Weapon currentWeapon{};

	EquipmentSet() = default;
	void equipWeapon(WeaponID);
};

void EquipmentSet::equipWeapon(WeaponID id)
{
	this->currentWeapon = Weapon(id);
	this->ContainsWeapon = true;
	weaponChanged();
}