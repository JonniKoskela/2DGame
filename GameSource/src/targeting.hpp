#pragma once

#include "targeting.h"

//template<typename T>
//T nextElement(std::vector<T>& data)
//{
//
//    return T;
//}






void TabTarget::tabTarget(){
    std::vector<Mob>& mobs = gameState.mobs.mobList;

    // päivitä distancevector
    //jos lista käyty läpi päivitä distance vector
    //päivittämisen jälkeen ota lähin mob
    static auto timer = std::make_unique<ElapseTimer>(350);

    if(!timer->elapsed())
    {
	  return;
    }

    if (distanceList.size() == 0 || cycleCounter == distanceList.size())
    {
	  cycleCounter = 0;
	  updateDistanceList();
    }

    currentTargetIndex = (currentTargetIndex + 1) % mobs.size();

    player.currentTarget = &mobs.at(currentTargetIndex);


    ++cycleCounter;
}



inline void TabTarget::reset()
{
    distanceList.clear();
    cycleCounter = 0;
    currentTargetIndex = 0;
}

inline void TabTarget::updateDistanceList()
{
    if (distanceList.size() == 0)
    {
	  for (Mob& mob : gameState.mobs.mobList)
	  {
		auto md = MobDistance(&mob, (int)distanceBetween(player.pos, mob.position));
		distanceList.push_back(md);
	  }
    }
    std::sort(distanceList.begin(),distanceList.end(), lessThanKey());
}