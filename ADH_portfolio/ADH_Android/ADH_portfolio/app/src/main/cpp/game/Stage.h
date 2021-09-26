#pragma once

#include "iStd.h"
#include "Character.h"

extern int stageFrom, stageTo;

void loadStage();
void freeStage();
void drawStage(float dt);
void keyStage(iKeyStat stat, iPoint point);

Texture* createTexStage();



