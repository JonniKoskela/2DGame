#pragma once
#include "equipment/equipmentSet.h"
#include "targeting.h"

class LocationEventObserver;
class TabTarget;

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
	WeaponRenderData weaponRenderData{};
	TabTarget target;
	Mob* currentTarget;

	EquipmentSet equipment{};

	const iVec2 size{31,31};
	Vec2 pos{};
	Vec2 renderPos{};
	Vec2 speed{};
	bool attacking{ false };
	bool moving{ false };
	std::unique_ptr<LocationEventObserver> LEObserver;

};
static Player player{};



//______________________________________________
iRect getPlayerRect(Player& player)
{
	iRect playerRect{};
	playerRect.pos.x = player.pos.x - (player.size.x / 2),
	playerRect.pos.y =	player.pos.y - (player.size.y / 2);

	playerRect.size.x = player.size.x;
	playerRect.size.y = player.size.y;

	
	return playerRect;
};

void weaponChanged()
{
	std::cout << "player weapon changed" << "\n";
	player.weaponRenderData.renderData.textureAtlasPosition = player.equipment.currentWeapon.weaponSprite.atlasOffset;
	player.weaponRenderData.renderData.textureAtlasOffset = player.equipment.currentWeapon.weaponSprite.size;
}