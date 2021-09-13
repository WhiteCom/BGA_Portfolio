#pragma once

#include "iStd.h"

//================================================
//popLoad
//================================================

void createPopLoad();
void freePopLoad();
void drawPopLoad(float dt);
void showPopLoad(bool show);
bool keyPopLoad(iKeyStat stat, iPoint point);

//================================================
//popSave
//================================================

void createPopSave();
void freePopSave();
void drawPopSave(float dt);
void showPopSave(bool show);
bool keyPopSave(iKeyStat stat, iPoint point);
