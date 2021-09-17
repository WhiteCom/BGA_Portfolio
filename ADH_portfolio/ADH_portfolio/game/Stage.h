#pragma once

#include "iStd.h"

extern int stageFrom, stageTo, step;

void loadStage();
void freeStage();
void drawStage(float dt);
void keyStage(iKeyStat stat, iPoint point);
