#pragma once
#include <vector>
#include "GLrenderer.h"
#include "attackActionStructs.h"
#include "attackTimer.h"
#include "../GLRenderer_attackRenderData.h"
#include "algorithm"
#include "../easing.hpp"

std::vector<Vec2> generateSlamVertices(Vec2& pos, float mAngle, float range, float slamTimer);
std::vector<Vec2> generateArcVertices(Vec2&, float, float);
attackRenderData4xVec2 generateMovingArcVertices(Vec2& pos, float mAngle, float armRotation, float handleRotation);

bool arcHitDetection(float AttackAngle);
//--------------------------------------------------------------------------ARC



void processArc(Action& action, bool renderer)
{
	float arcTimer = action.attackTimer.coolDownTimer;
	if (arcTimer < action.attackTimer.renderTime)
	{
	static std::vector<Vec2> arcVertices{};

		if (arcTimer == 0.0f)
		{
			Vec2 normalizedmPos = normalizeTo(player.pos, mPos);
			float angle = atan2f(normalizedmPos.x, normalizedmPos.y);
			arcVertices = generateArcVertices(player.pos, angle, 35.0f);
			arcRenderData(arcVertices, arcTimer);
		}
		else
		{
			arcRenderData(arcVertices, arcTimer);
		}
	}

	return;
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

bool arcHitDetection(float AttackAngle)
{
	Vec2 normalizedMonsterPos = normalizeTo(player.pos, gobo.position);
	float monsterPlayerAngle = atan2f(normalizedMonsterPos.x, normalizedMonsterPos.y);
	if (AttackAngle - 1 <= monsterPlayerAngle && AttackAngle + 1 >= monsterPlayerAngle && distanceBetween(gobo.position, player.pos) <= 85.0f)
	{
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------SLAM
void processSlam(Action& action,bool renderer)
{
	float slamTimer = action.attackTimer.coolDownTimer;
	std::cout << slamTimer << "\n";
	Vec2 normalizedmPos = normalizeTo(player.pos, mPos);
	float angle = atan2f(normalizedmPos.x, normalizedmPos.y);
	if (slamTimer < action.attackTimer.dynamic_attackMaxTime)
	{
		std::vector<Vec2> slamVertices = generateSlamVertices(player.pos, angle, 30.0f, slamTimer);
		slamRenderData(slamVertices, slamTimer, player.pos);
		Matrix3f transform{};
		transform.translate(50, 0);
		transform.rotate(1);
		for (Vec2& vertex : slamVertices)
		{
			vertex = vertex * transform;
		}
	}
	else
	{
		renderSlamBool = false;
	}
	if (slamTimer > 0.2f)
	{
		std::cout << slamTimer << "\n";
	}
	//else
	//{
	//	slamHitDetection();
	//}
}
std::vector<Vec2> generateSlamVertices(Vec2& pos, float mAngle, float range, float slamTimer)
{
	std::vector<Vec2> vertices{};
	vertices.reserve(4);
	float angle = mAngle + 0.5f * fPi;
	float distance = 30.0f;
	float width = 50.0f;
	float endRange = distance + (range * slamTimer);
	float innerAngle = tanf(width * 0.5f / distance);
	float outerAngle = tanf(width * 0.5f / endRange);	
	Matrix3f transform{};
	transform.translate(100, 50);
	transform.rotate(1);

	vertices.push_back(Vec2{ pos.x + distance * cosf(angle - innerAngle), pos.y - distance * sinf(angle - innerAngle) });
	vertices.push_back(Vec2{ pos.x + distance * cosf(angle + innerAngle), pos.y - distance * sinf(angle + innerAngle) });
	vertices.push_back(Vec2{ pos.x+1 + endRange * cosf(angle + outerAngle), pos.y - endRange * sinf(angle + outerAngle) });
	vertices.push_back(Vec2{ pos.x+1 + endRange * cosf(angle - outerAngle), pos.y - endRange * sinf(angle - outerAngle) });

	return vertices;
}



struct movingArcData
{

};
void processMovingArc(Action& action, bool renderer)
{
	static attackRenderData4xVec2 vertices{};
	static bool init{false};
	static float angle;
	if (!init)
	{
		init = true;
		Vec2 normalizedmPos = normalizeTo(player.pos, mPos);
		angle = atan2f(normalizedmPos.x, normalizedmPos.y);
	}
	if (renderer){
		if (action.attackTimer.coolDownTimer <= action.attackTimer.renderTime)
		{

			float armRotation = easing::easeOutCirc((action.attackTimer.coolDownTimer + renderTimer) / action.attackTimer.activeTime);
			float handleRotation = easing::easeInExpo((action.attackTimer.coolDownTimer + renderTimer) / (action.attackTimer.activeTime+ action.attackTimer.backSwingTime));
			//float handleRotation = 0;
			std::cout << "armrotation: " << action.attackTimer.coolDownTimer / action.attackTimer.renderTime << "\n";
			vertices = generateMovingArcVertices(player.pos, angle, armRotation, handleRotation);
			action.currentVertices = &vertices;
			attackTransforms.push_back(vertices);
		}
		else init = false;
	}
}
attackRenderData4xVec2 generateMovingArcVertices(Vec2& originPoint, float mAngle, float armRotation, float handleRotation)
{
	float width = 13.0f;
	attackRenderData4xVec2 maVertices{};
	maVertices.vertices[0] = { 0.0f, -(width/2) };
	maVertices.vertices[1] = { 0.0f, (width/2) };
	maVertices.vertices[2] = { 35.0f, (width/2) };
	maVertices.vertices[3] = { 35.0f, -(width/2) };

	Matrix3f transform{};
	transform.translate(20.0f, 15.0f);
	transform.rotate(-mAngle+(1.1f*MPI+armRotation));
	for (auto& vert : maVertices.vertices)
	{
		vert = vert * transform;
		vert = vert + originPoint;
	}
	maVertices.rotateAroundHandle(handleRotation*0.6f);
	std::cout << "renderTimer: " << renderTimer << "\n";

	return maVertices;
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

