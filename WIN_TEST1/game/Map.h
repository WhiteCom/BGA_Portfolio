#pragma once

#include "iStd.h"
#include "Tile.h"
#include "Character.h"

//타일 맵을 그려야함
void loadMap();
void freeMap();
void drawMap(float dt, const char* str);
void keyMap(iKeyStat stat, iPoint point);