#pragma once
#include <bitset>
#include "mapEnum.h"
#include <array>

class MapLayout
{
public:
	MapLayout(MAP_SCENEID id)
	{
		switch (id)
		{
		case START_AREA:
			mapMaxWidth = 2;
			mapMaxHeight = 2;
			
			
			break;

		
		}
		assert((mapMaxWidth <= 4 && mapMaxHeight <= 4) && "map exceeded size limitation");
	}
	MapLayout() = default;

private:
	uint8_t mapMaxWidth{ 0 };
	uint8_t mapMaxHeight{ 0 };
	std::array<std::bitset<4>, 4> mapLayout{};
};