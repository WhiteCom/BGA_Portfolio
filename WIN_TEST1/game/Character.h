#pragma once

#include "iStd.h"
#include "TileType.h"
#include "Map.h"

void loadCharacter(iPoint off);
//void drawCharacter(float dt, MapTile* currMap);
void drawCharacter(float dt, FObject* fObj);
void freeCharacter();
void keyCharacter(iKeyStat stat, iPoint point);