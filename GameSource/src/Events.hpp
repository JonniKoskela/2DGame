#pragma once
#include <functional>
#include <map>
#include "utils.h"
#include "map/map.hpp"

struct EventBorders
{
	Vec2 xCoordinates;
	Vec2 yCoordinates;
};

class eventList
{
	std::map<EventBorders, std::function<void()>> eventList;
};

struct stageList
{
	std::map<MAP::MAP_ID, eventList> stageEvents;
};

class LocationEvent
{
	std::function<void()> eventFunction;
};

class LocationEventObserver
{
	eventList d;
};