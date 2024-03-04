#pragma once
#include "../fileUtils.h"
#include "mapEnum.h"
#include "mapMisc.h"
#include <fstream>
#include "mapFilePaths.h"
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
constexpr uint8_t MAP_TILESIZE = 32;

void loadMapChunks(MAP_SCENEID, std::vector<MapChunk>&);
void processMapChunk(std::vector<MapChunk>& mapChunks);


struct MapLayoutCoordinate
{
	uint8_t x;
	uint8_t y;

	MapLayoutCoordinate(int right, int up)
	{
		x = right;
		y = up;
	}
}typedef MAP_POSITION;


class MapChunk
{
public:
	MapLayoutCoordinate ChunkPosition;
	//map chunk size: 960x960px
	uint8_t tileLayer[30][30];
	bool collision[30][30];
};


class MapScene
{
public:
	std::vector<MapChunk> mapChunks;

	MapScene() = default;
	MapScene(MAP_SCENEID id)
	{
		switch (id)
		{
		case START_AREA:
			mapChunks = loadMapScene(id);
			break;
		}
	}

private:

	std::vector<MapChunk> loadMapScene(MAP_SCENEID);
};

std::vector<MapChunk> MapScene::loadMapScene(MAP_SCENEID id)
{
	std::vector<MapChunk> mapChunks{};

	loadMapChunks(id,mapChunks);

	return mapChunks;
}

// load files and parse into mapChunks

void loadMapChunks(MAP_SCENEID id, std::vector<MapChunk>& mapChunks)
{
	std::ifstream fstream;

	switch (id)
	{
	case START_AREA:
		
	}
}

void processMapChunk(std::vector<MapChunk>& mapChunks)
{
	std::ifstream fstream{};



}


void drawMap(const GameState& gameState)
{
	//
}