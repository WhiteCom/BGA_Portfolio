#pragma once

#include "iStd.h"

void loadMenu();
void freeMenu();
void drawMenu(float dt);
void keyMenu(iKeyStat stat, iPoint point);

//===========================================================
//popMenu
//===========================================================

void createPopMenu();
void freePopMenu();
void drawPopMenu(float dt);
void showPopMenu(bool show);
bool keyPopMenu(iKeyStat stat, iPoint point);

//===========================================================
//popHow
//===========================================================

void createPopHow();
void freePopHow();
void drawPopHow(float dt);
void showPopHow(bool show);
bool keyPopHow(iKeyStat stat, iPoint point);

//===========================================================
//popOption
//===========================================================
extern int soundSize;

void createPopOption();
void freePopOption();
void drawPopOption(float dt);
void showPopOption(bool show);
bool keyPopOption(iKeyStat stat, iPoint point);

//===========================================================
//popExit
//===========================================================

void createPopExit();
void freePopExit();
void drawPopExit(float dt);
void showPopExit(bool show);
bool keyPopExit(iKeyStat stat, iPoint point);


