#pragma once

#include "iStd.h"
#include "TileType.h"
#include "Map3.h"

void loadCharacter(iPoint off);
//void drawCharacter(float dt, MapTile* currMap);
void drawCharacter(float dt);
void freeCharacter();
void keyCharacter(iKeyStat stat, iPoint point);