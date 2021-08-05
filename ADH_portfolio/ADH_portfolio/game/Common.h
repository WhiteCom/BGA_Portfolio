#pragma once

#include "iStd.h"
#include "TileType.h"

struct AppData
{
	float eff, bgm;

	// unit info
	
	// item info

	//char mapData[MAP_FILE_SIZE * 10];
	char* mapData;

	AppData();
	~AppData();
};

extern AppData* appData;
#define APP_DATA_PATH "save.dat"
#define MAP_NUM 9

void loadMapData();
void freeAppData();
void callAppData();
void callMapData();
void saveAppData();

