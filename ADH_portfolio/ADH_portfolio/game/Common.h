#pragma once

#include "iStd.h"
#include "TileType.h"


struct AppData
{
	float eff, bgm;

	// unit info

	// item info

	char mapData[MAP_FILE_SIZE * 10];
};

extern AppData* appData;
#define APP_DATA_PATH "save.dat"

void callAppData();
void saveAppData();

