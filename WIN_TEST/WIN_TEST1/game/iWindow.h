#pragma once

// Windows ��� ����
#include <windows.h>

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

void startApp();
void drawApp();
void freeApp();
void endApp();