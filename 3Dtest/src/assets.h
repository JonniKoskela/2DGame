#pragma once
#include "utils.h"

enum SpriteID {
	SPRITE_FROG,
	SPRITE_DOOR,
};

class Sprite {
public:
	iVec2 size{};
	iVec2 atlasOffset{};
};

Sprite getSprite(SpriteID spriteID){
	Sprite sprite{};

	switch (spriteID)
	{
	case SPRITE_FROG:
	{
		sprite.atlasOffset = { 0,0 };
		sprite.size = { 32,32 };
		break;
	}
	case SPRITE_DOOR:
	{
		sprite.atlasOffset = { 128,0 };
		sprite.size = { 300,300 };
		break;
	}
	}
	return sprite;
}