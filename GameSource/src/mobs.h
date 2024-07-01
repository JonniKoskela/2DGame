#pragma once
#include "utils.h"


enum MOB_ID
{
	MOB_GOBLIN,
	MOB_DEFAULT,
};

uint64_t createUUID()
{
    return 1;
}






class Mob
{
public:
      uint16_t uniqueID = 0;

	MOB_ID id;
	uint32_t health;
	uint32_t damage;
	Vec2 speed;
	Vec2 position;
	Vec2 size{32.f,32.f};

	Mob() = default;
};

struct MobState
{
    std::vector<Mob> mobList;
};


Mob createMob(MOB_ID mobId, Vec2 position = {0.0f,0.0f})
{
	Mob mob{};
	static int mobID = 0;
	switch (mobId)
	{
	case MOB_GOBLIN:
	{
	     
		mob.size = { 18,35 };
		mob.health = 100;
		mob.damage = 10;
	}
	}
	mob.position = position;
	mob.id = mobId;
	return mob;
}

