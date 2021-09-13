#pragma once

#include "iStd.h"

#define EDITOR_USE_FBO 1

extern HWND hWnd;

extern Texture* weiFBO;
extern Texture* texFBO;
extern int selectedTexIdx;
extern int* tmpTileWei;
extern int selectedWei; //커서로 선택한 가중치
extern int selectedWeiType; //0 : tile, 1 : enemy, 2 : item
extern int selectedBtn;

extern iImage*** imgBtn;

void loadMapEditor();
void drawMapEditor(float dt);
void freeMapEditor();
void keyMapEditor(iKeyStat stat, iPoint point);

char* openFile(bool open, LPCWSTR filter);
