#pragma once
#include "../fileUtils.h"
#include "../utils.h"
#include <array>

//void readMapFile();
//void generateFile();
//
//static BumpAllocator mapBuffer;

//void loadMapFiles();
//void drawCurrentMap();

/*
	-need drawMap(gameState) function so that
	
	-need changeMap function
	*/

namespace MAP
{
	enum MAP_ID
	{
		MAP_START,
		MAP_DESERT,
	};


	std::string getMapFilePath(MAP_ID);


	//mapChunk = (32x32 tiles) columns first
	class Map
	{
	public:
		uint8_t map_width{};
		uint8_t map_height{};
		MAP_ID stageID;
		
		Map _initMap(MAP_ID);
	};

	Map Map::_initMap(MAP_ID id)
	{
		Map map;
		std::ifstream ifs;
		std::string fileStr = getMapFilePath(id);
		ifs.open(fileStr);
		std::stringstream ss;
		ss << ifs.rdbuf();
		std::string buffer;
		ss >> buffer;

		size_t itStart = buffer.find_first_of('{');
		size_t itEnd = buffer.find_first_of('}');
		std::string whbuffer;
		while (itStart != itEnd && itStart != ',')
		{

		}
		ifs.close();
	}










	std::string getMapFilePath(MAP_ID id)
	{
		switch (id)
		{
		case MAP_START:
			return "MAP_START.SMAP";
			break;
		}

	}
}
