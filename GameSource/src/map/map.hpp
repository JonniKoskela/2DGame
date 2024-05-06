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
		GLSL_uint map_width{};
		GLSL_uint map_height{};
		std::vector<uint32_t> tileLayer;
		MAP_ID stageID;

		static Map _initMap(MAP_ID id);
	};
	Map Map::_initMap(MAP_ID id)
	{
		int x = 0;
		int y = 0;
		Map map{};
		
		std::string fp = getMapFilePath(id);

		std::ifstream ifs(fp);
		if (!ifs)
		{
			std::cout << "failed to open file, map.hpp";
		}
		assert(ifs.good());
		std::stringstream sstream;
		sstream << ifs.rdbuf();

		std::string buf;
		sstream >> buf;
		assert((buf[0] == '{' && buf[buf.length() - 1] == '}'));
		std::string::iterator bufIT = buf.begin() + 1;

		std::string::size_type sz{};
		std::string numberBuffer = std::string(buf.begin() + 1, buf.begin() + buf.find(','));
		x = std::stoi(numberBuffer);
		numberBuffer = std::string(buf.begin() + buf.find(',') + 1, buf.begin() + buf.find('}'));
		y = std::stoi(numberBuffer);

		std::cout << x << " " << y << "\n";
		map.map_width = x;
		map.map_height = y;
		//sstream >> buf;
		std::string tiles;

		std::string token;
		while (std::getline(sstream, tiles, '_'))
		{
			std::stringstream tileStream(tiles);
			while (std::getline(tileStream, token, ','))
			{
				map.tileLayer.push_back(std::stoi(token));
			}
		}

		for (int i : map.tileLayer)
		{
			std::cout << i << "\n";
		}

		std::cout << "map loaded" << std::endl;
		std::cout << x + y << std::endl;
		ifs.close();

		return map;
	}










	std::string getMapFilePath(MAP_ID id)
	{
		switch (id)
		{
		case MAP_START:
			return "./assets/mapFiles/MAP_START.SMAP";
			break;
		}

	}
}
