#pragma once
#include "equipment/equipmentSet.h"
enum WeaponDisplacement
{
	WEAPON_DISPLACEMENT_LEFT,
};

enum PlayerCurrentActionStateID
{
	PLAYER_IDLE,
	PLAYER_ATTACKING,
};

//is used to display where the players weapon is etc;
class WeaponRenderData
{
public:
	WeaponDisplacement attackStance{WEAPON_DISPLACEMENT_LEFT};
	RenderData4xVec2 renderData{};

	WeaponRenderData() = default;
};

class Player
{
public:
	Vec2 pos{};
	Vec2 renderPos{};
	Vec2 speed{};
	EquipmentSet equipment{};
	WeaponRenderData weaponRenderData{};
	bool attacking{ false };
	bool moving{ false };

};
static Player player{};

void weaponChanged()
{
	std::cout << "player weapon changed" << "\n";
	player.weaponRenderData.renderData.textureAtlasPosition = player.equipment.currentWeapon.weaponSprite.atlasOffset;
	player.weaponRenderData.renderData.textureAtlasOffset = player.equipment.currentWeapon.weaponSprite.size;
}