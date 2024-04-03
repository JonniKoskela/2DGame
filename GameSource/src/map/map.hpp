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


	class Map
	{
	public:
		uint8_t map_width{};
		uint8_t map_height{};
		std::vector <uint16_t> tileLayer;
		MAP_ID stageID;
		
		Map _initMap(MAP_ID);
	};

	Map Map::_initMap(MAP_ID id)
	{
		Map map;
		std::vector<uint16_t> vec;
		std::ifstream ifs(getMapFilePath(id));
		if (!ifs)
		{
			std::cout << "fail";
		}
		std::stringstream sstream;
		sstream << ifs.rdbuf();

		std::string buf;
		sstream >> buf;
		assert((buf[0] == '{' && buf[buf.length() - 1] == '}'));
		std::string::iterator bufIT = buf.begin() + 1;

		std::string::size_type sz{};
		std::string numberBuffer = std::string(buf.begin() + 1, buf.begin() + buf.find(','));
		map.map_width = std::stoi(numberBuffer);
		numberBuffer = std::string(buf.begin() + buf.find(',') + 1, buf.begin() + buf.find('}'));
		map.map_height = std::stoi(numberBuffer);


		//sstream >> buf;

		std::string token;
		while (std::getline(sstream, token, ','))
		{
			map.tileLayer.push_back(std::stoi(token));
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
