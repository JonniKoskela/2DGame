#pragma once
#include <vector>
#include "GLrenderer.h"

constexpr float fPi = 3.141592653589793;

using Duration = std::chrono::duration<float>;
using Clock = std::chrono::steady_clock;
auto previousArcTime = Clock::now();
auto currentArcTime = Clock::now();

int arcVertexCapacity{ 25 };
float arcTimer{ 0.0f };
float arcFadeTime = 500.0f;
bool attacking = false;
extern Mob gobo;

void resetTimer();
bool arcHitDetection(float AttackAngle);
int performingAttackType;


  
struct ArcVertex
{
	Vec2 vertex;
};
std::vector<ArcVertex> generateArcVertices(Vec2& pos, float mAngle, float distance);
void genArcBuffer(std::vector<ArcVertex> arcVertices);
void startArcAttack();

struct SlamVertex
{
	Vec2 vertex;
};
std::vector<SlamVertex> generateSlamVertices(Vec2& pos, float mAngle, float range);
void genSlamBuffer(std::vector<SlamVertex> slamVertices);
void startSlamAttack();





enum AttackID
{
	ARC_ATTACK,
	SLAM_ATTACK,
};


class ActionBar
{
public:
	std::vector<AttackID> actions{};
	void startAttack(AttackID attackID);
};

void ActionBar::startAttack(AttackID attackID)
{
	switch (attackID)
	{
	case ARC_ATTACK:
		startArcAttack();
		performingAttackType = ARC_ATTACK;
		break;

	case SLAM_ATTACK:
		startSlamAttack();
		performingAttackType = SLAM_ATTACK;
		break;
	}
}

void startArcAttack()
{
	resetTimer();
	attacking = true;
	Vec2 normalizedmPos = normalizeTo(player.pos, mPos);
	float angle = atan2f(normalizedmPos.x, normalizedmPos.y);

	if (arcHitDetection(angle) == true)
		std::cout << "hit" << "\n";

	std::vector<ArcVertex> arcVertices = generateArcVertices(player.pos, angle, 35.0f);
	genArcBuffer(arcVertices);
	std::cout << distanceBetween(gobo.position, player.pos);
}




//--------------------------------------------------------------------------ATTACKS
std::vector<ArcVertex> generateArcVertices(Vec2& pos, float mAngle, float distance)
{
	std::vector<ArcVertex> vertices{};
	vertices.reserve(arcVertexCapacity);
	//float centerX = normalize();
	float radius = 50.0f + distance;
	float startAngle = mAngle + 1.5*fPi ;
	float centerX = pos.x + cosf(startAngle) * distance;
	float centerY = pos.y - sinf(startAngle) * distance;
	//float startAngle = fPi;
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
	std::cout << Vec2{ pos.x + distance * cosf(1), pos.y - distance * sinf(mAngle - 1) }<< "\n";

	return vertices;
}



std::vector<SlamVertex> generateSlamVertices(Vec2& pos, float mAngle, float range)
{
	std::vector<SlamVertex> vertices{};
	vertices.reserve(4);
	float angle = mAngle + 1.5f*fPi;
	float distance = 15.0f;
	float endRange = distance + range;
	vertices.push_back(SlamVertex{ pos.x + distance * cosf(angle - 1), pos.y - distance * sinf(angle - 1)});
	vertices.push_back(SlamVertex{ pos.x + distance * cosf(angle + 1), pos.y - distance * sinf(angle + 1)});
	vertices.push_back(SlamVertex{ pos.x + endRange * cosf(angle + 0.25f), pos.y - endRange * sinf(angle + 0.25f) });
	vertices.push_back(SlamVertex{ pos.x + endRange * cosf(angle - 0.25f), pos.y - endRange * sinf(angle - 0.25f) });
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
	if (arcTimer >= arcFadeTime)
	{
		attacking = false;
	}
	return elapsed.count();
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

void startSlamAttack()
{
	resetTimer();
	attacking = true;
	Vec2 normalizedmPos = normalizeTo(player.pos, mPos);
	float angle = atan2f(normalizedmPos.x, normalizedmPos.y);

	if (arcHitDetection(angle) == true)
		std::cout << "hit" << "\n";

	std::vector<SlamVertex> slamVertices = generateSlamVertices(player.pos, angle, 70.0f);
	genSlamBuffer(slamVertices);
	std::cout << distanceBetween(gobo.position, player.pos);
}