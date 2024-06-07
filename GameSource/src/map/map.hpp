#pragma once

#include "../fileUtils.h"
#include "../utils.h"
#include <array>
#include <bitset>
#include <map>
#include <algorithm>
#include "../game.h"
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
	};


	std::string getMapFilePath(MAP_ID);


	class Map
	{
	public:
		GLSL_uint map_width{};
		GLSL_uint map_height{};
		bool gameGrid[80][80];
		std::vector<uint32_t> tileLayer;
		std::bitset<1000> collisionLayer;

		MAP_ID stageID;

		static Map _initMap(MAP_ID id);
		static std::bitset<1000> generateCollisionLayer(Map&);
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
		map.collisionLayer = Map::generateCollisionLayer(map);
		return map;
	}





	std::bitset<1000> Map::generateCollisionLayer(Map& map)
	{
		std::vector<uint32_t> collidingTiles{ 1, };
		std::bitset<1000> collisionLayer;
		size_t t = 0;
		size_t i = 0;
		size_t j = 0;
		
		for (uint32_t& n : map.tileLayer)
		{
			if (std::find(collidingTiles.begin(),collidingTiles.end(),n) != collidingTiles.end())
			{
				collisionLayer.set(t);
				map.gameGrid[j][i] = true;
			}
			++j;
			if (j >= map.map_width)
			{
				 j = 0; ++i;
			}
			
		}

		

		return collisionLayer;
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
