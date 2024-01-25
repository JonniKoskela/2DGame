#pragma once
//#include "GLrenderer.h"
#include <vector>

constexpr float fPi = 3.141592653589793;

using Duration = std::chrono::duration<float>;
using Clock = std::chrono::steady_clock;
auto previousArcTime = Clock::now();
auto currentArcTime = Clock::now();

int arcVertexCapacity{ 25 };
float arcTimer{ 0.0f };
float arcFadeTime = 500.0f;
bool drawArc = false;

struct ArcVertex
{
	Vec2 vertex;
};





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
	vertices.push_back(ArcVertex{ pos.x +cosf(startAngle +1 )* distance, pos.y - sinf(startAngle +1 ) * distance });
	vertices.push_back(ArcVertex{ pos.x + cosf(startAngle -1) * distance, pos.y - sinf(startAngle -1) * distance });
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



void generateSlamAttack(Vec2 pos, float mAngle)
{

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
		drawArc = false;
	}
	return elapsed.count();
}
