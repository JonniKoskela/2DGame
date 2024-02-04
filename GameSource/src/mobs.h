#pragma once
#include "utils.h"

enum MOB_ID
{
	MOB_GOBLIN,
};

class Mob
{
public:
	int health{};
	int damage{};
	Vec2 speed{};
	Vec2 position{};
	Vec2 size{};
};

Mob createMob(MOB_ID mobId, Vec2 position = {0.0f,0.0f})
{
	Mob mob{};
	switch (mobId)
	{
	case MOB_GOBLIN:
	{
		mob.size = { 18,35 };
		mob.health = 100;
		mob.damage = 10;
	}
	}

	return mob;
}