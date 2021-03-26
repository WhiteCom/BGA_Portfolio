#pragma once

#include "iStd.h"
#include "Tile.h"


void loadCharacter(iPoint off);
void drawCharacter(float dt, MapTile* currMap);
void freeCharacter();
bool moveCheck(MapTile* map); //���� ����ġ�� ���� �����ϼ����� ���� �Ǵ�
void keyCharacter(iKeyStat stat, iPoint point);