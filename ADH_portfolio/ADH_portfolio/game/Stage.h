#pragma once

#include "iStd.h"
#include "Character.h"


extern int stageFrom, stageTo;
extern Character* mainCharacter;

void loadStage();
void freeStage();
void drawStage(float dt);
void keyStage(iKeyStat stat, iPoint point);
