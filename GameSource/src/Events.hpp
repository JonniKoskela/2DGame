#pragma once
#include <functional>
#include <map>
#include "utils.h"
#include "map/map.hpp"
#include "iostream";
#include <utility>





struct EventBorders
{
	Vec2 xCoordinates;
	Vec2 yCoordinates;
};
struct LocationEventData
{
	EventBorders borders;
	std::function<void()> eventFn;
	bool detachObserver = false;
	LocationEventData() = default;
};
void triggerEvent(LocationEventData&);
class eventList
{
public:
	std::vector<LocationEventData> eventList;
};

struct stageList
{
	std::map<MAP::MAP_ID, eventList> stageEvents;
};

class LocationEvent
{
public:
	std::function<void()> eventFunction;
};
void testEvent()
{
	std::cout << "aaaaaa" << std::endl;
}

EventBorders testBorders = { {0,100}, {0,100} };

static eventList testEventList;

class LocationEventObserver
{
public:
	bool active;
	eventList currentStageEvents;

	LocationEventObserver(eventList& el)
	{
		static bool init = false;

		if (!init)
		{
			init = true;
			
			//el.eventList.push_back(entry);
		}
		currentStageEvents = el;
	}

	void updateEventStatus();
};

void LocationEventObserver::updateEventStatus()
{
	for (auto& event : this->currentStageEvents.eventList)
	{
		bool insideX = (player.pos.x > event.borders.xCoordinates.x) && (player.pos.x < event.borders.xCoordinates.y);
		bool insideY = (player.pos.y > event.borders.yCoordinates.x) && (player.pos.y < event.borders.yCoordinates.y);
		if (insideX && insideY)
		{
			triggerEvent(event);
		}
	}
}

void triggerEvent(LocationEventData& event)
{
	event.eventFn();

	if (event.detachObserver)
	{
		player.LEObserver->active = false;
	}
}


