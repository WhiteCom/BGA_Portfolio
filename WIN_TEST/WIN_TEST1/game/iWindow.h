#pragma once

// Windows 헤더 파일
#include <windows.h>

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;


#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#include "iType.h"


extern Graphics* graphics;

ULONG_PTR startApp(HDC hdc, VOID_METHOD m);
void drawApp(FLOAT_METHOD m);
void endApp(ULONG_PTR token, VOID_METHOD m);
void resizeApp(int width, int height);
void clearApp();
iPoint coordinate(int x, int y);

wchar_t* utf8_to_utf16(const char* szFormat, ...);
char* utf16_to_utf8(const wchar_t* str);