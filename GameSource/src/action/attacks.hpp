#pragma once
#include <vector>
#include "GLrenderer.h"
#include "attackActionStructs.h"
#include "attackTimer.h"

std::vector<attackVertex> generateSlamVertices(Vec2& pos, float mAngle, float range, float slamTimer);
std::vector<attackVertex> generateArcVertices(Vec2&, float, float, float);

bool arcHitDetection(float AttackAngle);
//--------------------------------------------------------------------------ARC



void processArc(Action& action)
{
	float arcTimer = action.attackTimer.coolDownTimer;
	if (arcTimer < action.attackTimer.renderTime)
	{
	std::vector<attackVertex> arcVertices{};

		if (arcTimer == 0.0f)
		{
			Vec2 normalizedmPos = normalizeTo(player.pos, mPos);
			float angle = atan2f(normalizedmPos.x, normalizedmPos.y);
			arcVertices = generateArcVertices(player.pos, angle, 70.0f, arcTimer);
			arcRenderData(arcVertices, arcTimer);
		}
		else
		{
			Vec2 normalizedmPos = normalizeTo(player.pos, mPos);
			float angle = atan2f(normalizedmPos.x, normalizedmPos.y);
			arcVertices = generateArcVertices(player.pos, angle, 70.0f, arcTimer);
			arcRenderData(arcVertices, arcTimer);
		}
	}

	return;
}
std::vector<attackVertex> generateArcVertices(Vec2& pos, float mAngle, float distance, float arcTimer)
{
	std::vector<attackVertex> vertices{};
	vertices.reserve(arcVertexCapacity);
	float radius = 50.0f + distance;
	float startAngle = mAngle + 0.5*fPi ;
	float centerX = pos.x + cosf(startAngle) * distance;
	float centerY = pos.y - sinf(startAngle) * distance;
	float endAngle = startAngle + 2;
	float angleIncrement = (endAngle - startAngle) / static_cast<float>(vertices.capacity() - 2);
	vertices.push_back(attackVertex{ pos.x + cosf(startAngle+1 )* distance, pos.y - sinf(startAngle+1 ) * distance });
	vertices.push_back(attackVertex{ pos.x + cosf(startAngle-1) * distance, pos.y - sinf(startAngle-1) * distance });
	for (int i = 0; i < vertices.capacity()-2; ++i)
	{
		attackVertex arcVertex{};
		float angle = (startAngle-1) + i * angleIncrement;
		arcVertex.vertex.x = pos.x + radius * cos(angle);
		arcVertex.vertex.y = pos.y - radius * sin(angle);

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
void processSlam(Action& action)
{
	float slamTimer = action.attackTimer.coolDownTimer;
	std::cout << slamTimer << "\n";
	Vec2 normalizedmPos = normalizeTo(player.pos, mPos);
	float angle = atan2f(normalizedmPos.x, normalizedmPos.y);
	if (slamTimer < action.attackTimer.dynamic_attackMaxTime)
	{
		std::vector<attackVertex> slamVertices = generateSlamVertices(player.pos, angle, 30.0f, slamTimer);
		slamRenderData(slamVertices, slamTimer);
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
std::vector<attackVertex> generateSlamVertices(Vec2& pos, float mAngle, float range, float slamTimer)
{
	std::vector<attackVertex> vertices{};
	vertices.reserve(4);
	float angle = mAngle + 0.5f * fPi;
	float distance = 30.0f;
	float width = 50.0f;
	float endRange = distance + (range * slamTimer);
	float innerAngle = tanf(width * 0.5f / distance);
	float outerAngle = tanf(width * 0.5f / endRange);

	vertices.push_back(attackVertex{ pos.x + distance * cosf(angle - innerAngle), pos.y - distance * sinf(angle - innerAngle) });
	vertices.push_back(attackVertex{ pos.x + distance * cosf(angle + innerAngle), pos.y - distance * sinf(angle + innerAngle) });
	vertices.push_back(attackVertex{ pos.x+1 + endRange * cosf(angle + outerAngle), pos.y - endRange * sinf(angle + outerAngle) });
	vertices.push_back(attackVertex{ pos.x+1 + endRange * cosf(angle - outerAngle), pos.y - endRange * sinf(angle - outerAngle) });
	return vertices;
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

