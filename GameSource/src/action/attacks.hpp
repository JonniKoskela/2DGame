#pragma once
#include <vector>
#include "GLrenderer.h"
#include "attackActionStructs.h"
#include "attackTimer.h"
#include "../GLRenderer_attackRenderData.h"
#include "algorithm"
#include "../easing.hpp"
#include "animationInterface.h"

std::vector<Vec2> generateSlamVertices(Vec2& pos, float mAngle, float range, float slamTimer);
std::vector<Vec2> generateArcVertices(Vec2&, float, float);
RenderData4xVec2 animateWeaponSwing(Vec2& pos, float mAngle, float armRotation, float handleRotation);
RenderData4xVec2 generateMovingArcAttackAnim(Vec2 const& playerPos, float mAngle, Action& action, bool init);
bool arcHitDetection(float AttackAngle);
//--------------------------------------------------------------------------ARC



void processArc(Action& action, bool renderer)
{
	//float arcTimer = action.actionTimer.coolDownTimer;
	//if (arcTimer < action.actionTimer.renderTime)
	//{
	//static std::vector<Vec2> arcVertices{};

	//	if (arcTimer == 0.0f)
	//	{
	//		Vec2 normalizedmPos = normalizeTo(player.pos, mPos);
	//		float angle = atan2f(normalizedmPos.x, normalizedmPos.y);
	//		arcVertices = generateArcVertices(player.pos, angle, 35.0f);
	//		arcRenderData(arcVertices, arcTimer);
	//	}
	//	else
	//	{
	//		arcRenderData(arcVertices, arcTimer);
	//	}
	//}

	//return;
}
std::vector<Vec2> generateArcVertices(Vec2& pos, float mAngle, float distance)
{
	std::vector<Vec2> vertices{};
	vertices.reserve(arcVertexCapacity);
	float radius = 10.0f + distance;
	float arcWidth = 0.8f;
	float startAngle = mAngle + 0.5*fPi ;
	float centerX = pos.x + cosf(startAngle) * distance;
	float centerY = pos.y - sinf(startAngle) * distance;
	float endAngle = startAngle + arcWidth;
	float angleIncrement = (endAngle - startAngle) / static_cast<float>(vertices.capacity() - 2);
	vertices.push_back(Vec2{ pos.x + cosf(startAngle+arcWidth/2 )* distance, pos.y - sinf(startAngle+arcWidth/2 ) * distance });
	vertices.push_back(Vec2{ pos.x + cosf(startAngle-arcWidth/2) * distance, pos.y - sinf(startAngle-arcWidth/2) * distance });
	for (int i = 0; i < vertices.capacity()-2; ++i)
	{
		Vec2 arcVertex{};
		float angle = (startAngle-arcWidth/2) + i * angleIncrement;
		arcVertex.x = pos.x + radius * cos(angle);
		arcVertex.y = pos.y - radius * sin(angle);

		vertices.push_back(arcVertex);
	}
	//std::cout << Vec2{ pos.x + distance * cosf(1), pos.y - distance * sinf(mAngle - 1) }<< "\n";

	return vertices;
}

//bool arcHitDetection(float AttackAngle)
//{
//	Vec2 normalizedMonsterPos = normalizeTo(player.pos, gobo.position);
//	float monsterPlayerAngle = atan2f(normalizedMonsterPos.x, normalizedMonsterPos.y);
//	if (AttackAngle - 1 <= monsterPlayerAngle && AttackAngle + 1 >= monsterPlayerAngle && distanceBetween(gobo.position, player.pos) <= 85.0f)
//	{
//		return true;
//	}
//	return false;
//}
//
////-----------------------------------------------------------------------SLAM
void processSlam(Action& action,bool renderer)
{
	//float slamTimer = action.actionTimer.coolDownTimer;
	//std::cout << slamTimer << "\n";
	//Vec2 normalizedmPos = normalizeTo(player.pos, mPos);
	//float angle = atan2f(normalizedmPos.x, normalizedmPos.y);
	//if (slamTimer < action.actionTimer.dynamic_attackMaxTime)
	//{
	//	std::vector<Vec2> slamVertices = generateSlamVertices(player.pos, angle, 30.0f, slamTimer);
	//	slamRenderData(slamVertices, slamTimer, player.pos);
	//	Matrix3f transform{};
	//	transform.translate(50, 0);
	//	transform.rotate(1);
	//	for (Vec2& vertex : slamVertices)
	//	{
	//		vertex = vertex * transform;
	//	}
	//}
	//else
	//{
	//	renderSlamBool = false;
	//}
	//if (slamTimer > 0.2f)
	//{
	//	std::cout << slamTimer << "\n";
	//}
	//else
	//{
	//	slamHitDetection();
	//}
}
//std::vector<Vec2> generateSlamVertices(Vec2& pos, float mAngle, float range, float slamTimer)
//{
//	//std::vector<Vec2> vertices{};
//	//vertices.reserve(4);
//	//float angle = mAngle + 0.5f * fPi;
//	//float distance = 30.0f;
//	//float width = 50.0f;
//	//float endRange = distance + (range * slamTimer);
//	//float innerAngle = tanf(width * 0.5f / distance);
//	//float outerAngle = tanf(width * 0.5f / endRange);	
//	//Matrix3f transform{};
//	//transform.translate(100, 50);
//	//transform.rotate(1);
//
//	//vertices.push_back(Vec2{ pos.x + distance * cosf(angle - innerAngle), pos.y - distance * sinf(angle - innerAngle) });
//	//vertices.push_back(Vec2{ pos.x + distance * cosf(angle + innerAngle), pos.y - distance * sinf(angle + innerAngle) });
//	//vertices.push_back(Vec2{ pos.x+1 + endRange * cosf(angle + outerAngle), pos.y - endRange * sinf(angle + outerAngle) });
//	//vertices.push_back(Vec2{ pos.x+1 + endRange * cosf(angle - outerAngle), pos.y - endRange * sinf(angle - outerAngle) });
//
//	//return vertices;
//}

void processMovingArc(Action& action,Player& player ,bool renderer)
{
	RenderData4xVec2 weaponAnimationVertices{};

	static RenderData4xVec2 flyingArcVertices{};
	static bool init{false};
	static float angle;

	if (!init)
	{
		Vec2 normalizedmPos = normalizeTo(player.pos, mPos);
		angle = atan2f(normalizedmPos.x, normalizedmPos.y);
		flyingArcVertices = generateMovingArcAttackAnim(player.pos, angle, action, 0);
		init = true;
	}
	if (renderer){
		if (action.actionTimer.coolDownTimer+renderTimer <= action.actionTimer.renderTime)
		{
			// attack projectile
			{
				flyingArcVertices = generateMovingArcAttackAnim(player.pos,angle,action, 1);
				int animationIdx = animate(action.actionTimer.coolDownTimer + renderTimer, action.drawData.frameCount, action.actionTimer.renderTime);
				push_animationFrame(animationIdx, action.drawData.animation_atlasPosition, action.drawData.animation_atlasSize, flyingArcVertices);

			}
			//attack swing
			{
				float handleRotation = easing::easeInExpo((action.actionTimer.coolDownTimer + renderTimer) / (action.actionTimer.activeTime + action.actionTimer.backSwingTime));
				float armRotation = easing::easeOutCirc((action.actionTimer.coolDownTimer + renderTimer) / action.actionTimer.activeTime);
				//std::cout << "armrotation: " << action.actionTimer.coolDownTimer / action.actionTimer.renderTime << "\n";
				weaponAnimationVertices = animateWeaponSwing(player.pos, angle, armRotation, handleRotation);
				action.currentVertices = &weaponAnimationVertices;
					weaponAnimationVertices.attackFlag = 1;
				weaponAnimationVertices.textureAtlasPosition = player.equipment.currentWeapon.weaponSprite.atlasOffset;
				weaponAnimationVertices.textureAtlasOffset = player.equipment.currentWeapon.weaponSprite.size;
				attackTransforms.push_back(weaponAnimationVertices);
			}
		}
		else
		{
			init = false;

		}
	}
}
//RenderData4xVec2 generateMovingArcSwing(Vec2& originPoint, float mAngle, float armRotation, float handleRotation)
//{
//
//}
RenderData4xVec2 animateWeaponSwing(Vec2& originPoint, float mAngle, float armRotation, float handleRotation)
{
	bool weaponOnLeft = player.weaponRenderData.playerWeaponOnLeft;
	auto offset = weaponOnLeft == false ? Vec2{ 20.0f,5.0f } : Vec2{ -25.0f,5.0f };
	float armRotationCalculated = weaponOnLeft == false ? -mAngle + (1.15f * MPI + armRotation) : -mAngle - (1.15f * MPI + armRotation);

	float width = player.equipment.currentWeapon.weaponSprite.size.y;
	RenderData4xVec2 maVertices{};
	maVertices.vertices[0] = { 0.0f };
	maVertices.vertices[1] = { 0.0f, (width/2) };
	maVertices.vertices[2] = { 16.0f, (width/2) };
	maVertices.vertices[3] = { 16.0f, -(width/2) };

	Matrix3f transform{};
	transform.translate(offset.x,offset.y);
	transform.rotate(armRotationCalculated);

	for (auto& vert : maVertices.vertices)
	{
		vert = vert * transform;
		vert = vert + originPoint;
	}

	float handleRotationCalc = weaponOnLeft == false ? handleRotation * 1.4f : -handleRotation+MPI * 1.4f;
	maVertices.rotateAroundHandle(handleRotationCalc);
	//std::cout << "renderTimer: " << renderTimer << "\n";

	return maVertices;
}

RenderData4xVec2 generateMovingArcAttackAnim(Vec2 const& playerPos,float mAngle, Action& action, bool init)
{
	float currentPhase = action.actionTimer.coolDownTimer+renderTimer / action.actionTimer.renderTime;
	constexpr float mouseOffset = 0.5f * MPI;
	RenderData4xVec2 verticesContainer{};
	static std::array<Vec2, 4> startVertices;
	static std::array<Vec2, 4> endVertices;
	if (!init)
	{
		float width = action.attackProperties->width;
		float attackRange = action.attackProperties->range;

		startVertices = 
		{
			Vec2{0.0f,-width/2},
			Vec2{0.0f, width/2},
			Vec2{10.0f, width/2},
			Vec2{10.0f, -width/2}
		};
		endVertices = 
		{
		Vec2{attackRange,-width / 2},
		Vec2{attackRange, width / 2},
		Vec2{10.0f + attackRange, width / 2},
		Vec2{10.0f + attackRange, -width / 2}
		};

		Matrix3f transform{};
		transform.rotate(-mAngle-mouseOffset);
		for (int i = 0;i<4;++i)
		{
			//startVertices[i] = startVertices[i] + player.pos;
			//endVertices[i] = endVertices[i] + player.pos;
			startVertices[i] = startVertices[i] * transform;
			endVertices[i] = endVertices[i] * transform;
		}
	}
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			verticesContainer.vertices[i].x = lerp(startVertices[i].x, endVertices[i].x, currentPhase) + player.pos.x;
			verticesContainer.vertices[i].y = lerp(startVertices[i].y, endVertices[i].y, currentPhase) + player.pos.y;
		}
	}

	return verticesContainer;
}



//---------------------------------------------------------------UTILS
//void resetTimer()
//{
//	arcTimer = 0.0f;
//	previousArcTime = Clock::now();
//	currentArcTime = Clock::now();
//}




//float getCurrentTime() 
//{
//	currentArcTime = Clock::now();
//	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentArcTime - previousArcTime);
//	previousArcTime = currentArcTime;
//	if (arcTimer >= arcFadeTime)
//	{
//		attacking = false;
//	}
//	return elapsed.count();
//}

