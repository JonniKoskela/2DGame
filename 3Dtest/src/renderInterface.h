#pragma once
#include <vector>
#include "fileUtils.h"
#include "utils.h"
#include "assets.h"

struct Transform {
	Vec2 size{ 0 };
	Vec2 pos{ 0 };
	iVec2 atlasOffset{ 0 };
	iVec2 spriteSize{ 0 };
};

struct orthographicCamera {
	Vec2 position{ 0 };
	Vec2 dimensions{ 0 };
	float zoom = 1.0f;

};


struct RenderData
{
	std::vector<Transform> transforms{};
	
	RenderData() {
		transforms.reserve(1000);
	}
};

static RenderData* renderData = new RenderData{};


void drawQuad(Vec2 pos, Vec2 size)
{
	Transform transform = {};
	transform.pos = pos - size / 2.0f;
	transform.size = size;
	transform.atlasOffset = { 32,0 };
	transform.spriteSize = { 1,1 };

	renderData->transforms.emplace_back(transform);
}
void drawQuad(Transform transform)
{
	renderData->transforms.emplace_back(transform);
}

void drawSprite(SpriteID spriteID, Vec2 pos)
{
	Sprite sprite = getSprite(spriteID);

	Transform transform = {};
	transform.pos = pos - vec_2(sprite.size) / 2.0f;
	transform.size = vec_2(sprite.size);
	transform.atlasOffset = sprite.atlasOffset;
	transform.spriteSize = sprite.size;

	renderData->transforms.push_back(transform);
}

void drawSprite(SpriteID spriteID, iVec2 pos)
{
	drawSprite(spriteID, vec_2(pos));
}