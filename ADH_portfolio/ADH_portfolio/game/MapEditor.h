#pragma once

#include "iStd.h"

#define EDITOR_USE_FBO 1

void loadMapEditor();
void drawMapEditor(float dt);
void freeMapEditor();
void keyMapEditor(iKeyStat stat, iPoint point);

//================================================
//imgBtn
//================================================

void createImgBtn();
void freeImgBtn();
void drawImgBtn(float dt);
bool keyImgBtn(iKeyStat stat, iPoint point);

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

