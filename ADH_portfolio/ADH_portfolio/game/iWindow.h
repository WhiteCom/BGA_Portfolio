#pragma once

#include "iGraphics.h"

#include "../resource.h"

#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
//#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif

extern Graphics* graphics;

void startApp(HDC hdc, VOID_METHOD m);
void drawApp(FLOAT_METHOD m);
void endApp(VOID_METHOD m);
void resizeApp(int width, int height);
iPoint coordinate(int x, int y);
