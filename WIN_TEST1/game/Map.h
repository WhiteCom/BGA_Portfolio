#pragma once

#include "iStd.h"
#include "Tile.h"
#include "Character.h"

//Ÿ�� ���� �׷�����
void loadMap();
void freeMap();
void drawMap(float dt, const char* str);
void keyMap(iKeyStat stat, iPoint point);