#pragma once
#include "utils.h"

enum SpriteID {
	SPRITE_FROG,
	SPRITE_DOOR,
	SPRITE_ARC,
	SPRITE_MOB_GOBLIN,
};

class Sprite
{
public:
	iVec2 size{};
	iVec2 atlasOffset{};
};

Sprite getSprite(SpriteID spriteID)
{
	Sprite sprite{};

	switch (spriteID)
	{
	case SPRITE_FROG:
	{
		sprite.atlasOffset = { 0,0 };
		sprite.size = { 31,31 };
		break;
	}
	case SPRITE_DOOR:
	{
		sprite.atlasOffset = { 128,0 };
		sprite.size = { 272,300 };
		break;
	}
	case SPRITE_MOB_GOBLIN:
	{
		sprite.atlasOffset = { 19,32 };
		sprite.size = { 18,35 };
		break;
	}

	}
	return sprite;
}

Sprite getMobSprite(MOB_ID spriteID)
{
    Sprite sprite{};

    switch (spriteID)
    {
    case MOB_GOBLIN:
    {
	  sprite.atlasOffset = { 19,32 };
	  sprite.size = { 18,35 };
	  break;
    }

    }
    return sprite;
}

 








/*
enum AttackID {
	ATTACK_ARC
};

class Attack {
public:
	iVec2 size{};
	iVec2 atlasOffset{};
};

Attack getAttack(AttackID attackID)
{
	Attack attack{};

	switch (attackID)
	{

	case ATTACK_ARC:
	{
		attack.atlasOffset = { 0, 52 };
		attack.size = { 15,48 };
	}
	return attack;
	}
}*/