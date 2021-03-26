#pragma once

#include "iStd.h"
#include "Tile.h"


void loadCharacter(iPoint off);
void drawCharacter(float dt, MapTile* currMap);
void freeCharacter();
bool moveCheck(MapTile* map); //맵의 가중치를 보고 움직일수없다 여부 판단
void keyCharacter(iKeyStat stat, iPoint point);