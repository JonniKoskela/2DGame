#pragma once
Vec2 toWorldCoordinates(Vec2 position)
{
	Vec2 cameraPosition{};
	cameraPosition.x = position.x;
	cameraPosition.y = position.y;

	return cameraPosition;
};


Vec2 mouseToWorld(Vec2 mousePos)
{
	return mousePos / 2;
}

