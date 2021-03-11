#pragma once

#include "iType.h"

#include "iFPS.h"
#include "iString.h"
#include "iPoint.h"
#include "iSort.h"
#include "iArray.h"
#include "iSize.h"
#include "iRect.h"
#include "iColor.h"

// C 런타임 헤더 파일입니다.
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#define DEV_WIDTH 1920
#define DEV_HEIGHT 1080
extern iSize devSize;
extern iRect viewport;

#define va_start_end(szText, szFormat) \
va_list args; \
va_start(args, szFormat); \
vsnprintf(szText, sizeof(szText), szFormat, args); \
va_end(args)

void setKeyDown(iKeyStat stat, int c);
int getKeyDown();

void appInitialize();
int random();

float _cos(float degree);
float _sin(float degree);

float uint8ToFloat(uint8 c);
uint8 floatToUint8(float c);

void setRGBA(iColor4f c);
void setRGBA(iColor4b c);
void setRGBA(float r, float g, float b, float a);
void getRGBA(float& r, float& g, float& b, float& a);

void clearRect();

int igImageWidth(igImage* ig);
int igImageHeight(igImage* ig);

igImage* createImage(const char* szFormat, ...);
void freeImage(igImage* img);
void drawImage(igImage* img, float x, float y, int anc,
	float ix, float iy, float iw, float ih,
	float rx, float ry,
	int xyz, float degree);
void drawImage(igImage* img, float x, float y, int anc);
