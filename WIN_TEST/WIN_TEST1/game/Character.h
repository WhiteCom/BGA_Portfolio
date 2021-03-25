#pragma once

#include "iStd.h"
#include "Tile.h"

void drawHero();
void freeHero();
bool moveCheck(MapTile* map); //맵의 가중치를 보고 움직일수없다 여부 판단
