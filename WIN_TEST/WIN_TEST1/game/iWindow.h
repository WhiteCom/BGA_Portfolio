#pragma once

// Windows 헤더 파일
#include <windows.h>

typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

void startApp();
void drawApp();
void freeApp();
void endApp();
void resizeApp(int width, int height);

wchar_t* utf8_to_utf16(const char* szFormat, ...);
char* utf16_to_utf8(const wchar_t* str);