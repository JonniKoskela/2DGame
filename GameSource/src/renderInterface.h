#pragma once
#include <vector>
#include "fileUtils.h"
#include "utils.h"
#include "assets.h"
#include "equipment/equipmentSet.h"
#include "GLRenderer_attackRenderData.h"
#include "player.h"
struct Transform {
	Vec2 size{ 0.0f };
	Vec2 pos{ 0.0f };
	iVec2 atlasOffset{ 0,0 };
	iVec2 spriteSize{ 0,0 };
};

struct OrtographicCamera {
	Vec2 position{ 0.0f,0.0f };
	Vec2 dimensions{ 0.0f,0.0f };
	float zoom = 1.0f;
};


struct RenderData
{
	std::vector<Transform> transforms{};
	OrtographicCamera gameCamera{};
	
	RenderData()
	{
		transforms.reserve(300);
	}
};

static RenderData* renderData = new RenderData{};


inline void drawQuad(Vec2 pos, Vec2 size)
{
	Transform transform = {};
	transform.pos = pos - size / 2.0f;
	transform.size = size;
	transform.atlasOffset = { 32,0 };
	transform.spriteSize = { 1,1 };

	renderData->transforms.emplace_back(transform);
}
inline void drawQuad(Transform transform)
{
	renderData->transforms.emplace_back(transform);
}

inline void drawSprite(SpriteID spriteID, Vec2 pos)
{
	Sprite sprite = getSprite(spriteID);

	Transform transform = {};
	transform.pos = pos - vec_2(sprite.size) / 2.0f;
	transform.size = vec_2(sprite.size);
	transform.atlasOffset = sprite.atlasOffset;
	transform.spriteSize = sprite.size;

	renderData->transforms.push_back(transform);
}

inline void drawSprite(SpriteID spriteID, iVec2 pos)
{
	drawSprite(spriteID, vec_2(pos));
}

inline void drawPlayerEquipment(const Player& player)
{
	RenderData4xVec2 weaponDraw = player.weaponRenderData.renderData;
	weaponDraw.attackFlag = 1;
	attackTransforms.push_back(weaponDraw);
}

void drawMob(MOB_ID mobid, Mob& mob)
{
    Transform transform{};

    Sprite mobSprite = getMobSprite(mobid);

    transform.atlasOffset = mobSprite.atlasOffset;
    transform.spriteSize = mobSprite.size;
    transform.size = mob.size;
    transform.pos = mob.position;

    renderData->transforms.push_back(transform);
}

void drawDebugQuad(Vec2 pos,int size)
{
    Transform transform{};


    transform.atlasOffset = { 32,32 };
    transform.spriteSize = {4,4};
    transform.size = { (float)size,(float)size };
    transform.pos = pos;

    renderData->transforms.push_back(transform);
}

void drawMobs()
{
    for (Mob& mob : gameState.mobs.mobList)
    {
	  drawMob(mob.id, mob);
    }
}


 //void drawAttack(AttackID attackID, Vec2 pos)
 //{
 //	Attack attack = getAttack(attackID);
 //
 //	Transform transform = {};
 //	transform.pos = pos - vec_2(attack.size) / 2.0f;
 //	transform.size = vec_2(attack.size) / 2.0f;
 //	transform.atlasOffset = attack.atlasOffset;
 //	transform.spriteSize = attack.size;
 //
 //	renderData->transforms.push_back(transform);
 //}


