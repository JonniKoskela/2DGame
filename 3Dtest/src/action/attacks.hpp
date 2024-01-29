#pragma once
#include <vector>
#include "GLrenderer.h"
#include "attackActionStructs.h"

std::vector<SlamVertex> generateSlamVertices(Vec2& pos, float mAngle, float range);
std::vector<ArcVertex> generateArcVertices(Vec2& pos, float mAngle, float distance);

bool arcHitDetection(float AttackAngle);
//--------------------------------------------------------------------------ARC
void startArcAttack()
{
	resetTimer();

	attacking = true;
	Vec2 normalizedmPos = normalizeTo(player.pos, mPos);
	float angle = atan2f(normalizedmPos.x, normalizedmPos.y);

	//if (arcHitDetection(angle) == true)
		//std::cout << "hit" << "\n";

	std::vector<ArcVertex> arcVertices = generateArcVertices(player.pos, angle, 35.0f);
	genArcBuffer(arcVertices);
	//std::cout << distanceBetween(gobo.position, player.pos);
}
std::vector<ArcVertex> generateArcVertices(Vec2& pos, float mAngle, float distance)
{
	std::vector<ArcVertex> vertices{};
	vertices.reserve(arcVertexCapacity);
	float radius = 50.0f + distance;
	float startAngle = mAngle + 0.5*fPi ;
	float centerX = pos.x + cosf(startAngle) * distance;
	float centerY = pos.y - sinf(startAngle) * distance;
	float endAngle = startAngle + 2;
	float angleIncrement = (endAngle - startAngle) / static_cast<float>(vertices.capacity() - 2);
	vertices.push_back(ArcVertex{ pos.x + cosf(startAngle+1 )* distance, pos.y - sinf(startAngle+1 ) * distance });
	vertices.push_back(ArcVertex{ pos.x + cosf(startAngle-1) * distance, pos.y - sinf(startAngle-1) * distance });
	for (int i = 0; i < vertices.capacity()-2; ++i)
	{
		ArcVertex arcVertex{};
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
void processArc()
{

}

//-----------------------------------------------------------------------SLAM
void startSlamAttack()
{
	resetTimer();
	attacking = true;
	Vec2 normalizedmPos = normalizeTo(player.pos, mPos);
	float angle = atan2f(normalizedmPos.x, normalizedmPos.y);

	//if (arcHitDetection(angle) == true)
	//	std::cout << "hit" << "\n";

	std::vector<SlamVertex> slamVertices = generateSlamVertices(player.pos, angle, 70.0f);
	genSlamBuffer(slamVertices);
	//std::cout << distanceBetween(gobo.position, player.pos);
}

std::vector<SlamVertex> generateSlamVertices(Vec2& pos, float mAngle, float range)
{
	std::vector<SlamVertex> vertices{};
	vertices.reserve(4);
	float angle = mAngle + 0.5f * fPi;
	float distance = 30.0f;
	float width = 50.0f;
	float endRange = distance + (range * arcTimer / attackFadeTime);
	float innerAngle = tanf(width * 0.5f / distance);
	float outerAngle = tanf(width * 0.5f / endRange);

	vertices.push_back(SlamVertex{ pos.x + distance * cosf(angle - innerAngle), pos.y - distance * sinf(angle - innerAngle) });
	vertices.push_back(SlamVertex{ pos.x + distance * cosf(angle + innerAngle), pos.y - distance * sinf(angle + innerAngle) });
	vertices.push_back(SlamVertex{ pos.x + endRange * cosf(angle + outerAngle), pos.y - endRange * sinf(angle + outerAngle) });
	vertices.push_back(SlamVertex{ pos.x + endRange * cosf(angle - outerAngle), pos.y - endRange * sinf(angle - outerAngle) });
	return vertices;
}


//---------------------------------------------------------------UTILS
void resetTimer()
{
	arcTimer = 0.0f;
	previousArcTime = Clock::now();
	currentArcTime = Clock::now();
}




float getCurrentTime() 
{
	currentArcTime = Clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentArcTime - previousArcTime);
	previousArcTime = currentArcTime;
	if (arcTimer >= attackFadeTime)
	{
		attacking = false;
	}
	return elapsed.count();
}

