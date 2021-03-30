#pragma once

#include "iStd.h"

#if 0
#include "Tile.h"
#include "Character.h"

//타일 맵을 그려야함
void loadMap();
void freeMap();
void newMap(); //새로운 맵
void mapTime(float dt);
void drawMap(float dt, const char* str);
void keyMap(iKeyStat stat, iPoint point);

#else

struct MObject
{
	const char* strPath;
	//uint16 img;
	iPoint position;
};

struct MTile
{
	uint16 img;
	uint8 weight;
	uint8 obj;// 0:none 1:tree 2:stone 3:
};

struct FObject
{
    uint8 index;
    iPoint position;

	void paint(float dt);
};

void loadMap();
void freeMap();
void drawMap(float dt, const char* str);
void keyMap(iKeyStat stat, iPoint point);

#endif