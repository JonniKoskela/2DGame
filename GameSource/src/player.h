#pragma once
#include "equipment/equipmentSet.h"

class LocationEventObserver;
enum WeaponDisplacement
{
	WEAPON_DISPLACEMENT_LEFT,
};

enum CurrentActionState
{
	PLAYER_IDLE,
	PLAYER_ATTACKING,
};

//is used to display where the players weapon is etc;
class WeaponRenderData
{
public:
	bool playerWeaponOnLeft{ false };
	RenderData4xVec2 renderData{};

	WeaponRenderData() = default;
};

class Player
{
public:
	Vec2 pos{};
	Vec2 renderPos{};
	Vec2 speed{};
	const iVec2 size{31,31};
	EquipmentSet equipment{};
	WeaponRenderData weaponRenderData{};
	bool attacking{ false };
	bool moving{ false };
	std::unique_ptr<LocationEventObserver> LEObserver;
};
static Player player{};

void weaponChanged()
{
	std::cout << "player weapon changed" << "\n";
	player.weaponRenderData.renderData.textureAtlasPosition = player.equipment.currentWeapon.weaponSprite.atlasOffset;
	player.weaponRenderData.renderData.textureAtlasOffset = player.equipment.currentWeapon.weaponSprite.size;
}