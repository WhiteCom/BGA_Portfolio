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
#include "iImage.h"

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

#define DEV_WIDTH 1280
#define DEV_HEIGHT 720
#define RATE_MONITOR 0.3f
extern iSize devSize;
extern iRect viewport;

#define va_start_end(szText, szFormat) \
va_list args; \
va_start(args, szFormat); \
vsnprintf(szText, sizeof(szText), szFormat, args); \
va_end(args)

extern int keyStat, keyDown; //stat : 계속 누름, down : 한번 누름
void setKeyStat(iKeyStat stat, unsigned int c);
void setKeyDown(iKeyStat stat, unsigned int c);

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
void setClip(float x, float y, float width, float height);

void drawRect(iRect rt, float radius = 0.0f);
void drawRect(float x, float y, float width, float height, float radius = 0.0f);

void fillRect(iRect rt, float ridius = 0.0f);
void fillRect(float x, float y, float width, float height, float radius = 0.0f);

int igImageWidth(igImage* ig);
int igImageHeight(igImage* ig);

Texture** createImageDivide(int numX, int numY, const char* szFormat, ...);
Texture* createImage(const char* szFormat, ...);
void freeImage(Texture* img);
void drawImage(Texture* img, float x, float y, int anc,
	float ix, float iy, float iw, float ih,
	float rx, float ry,
	int xyz, float degree, int reverse = REVERSE_NONE);
void drawImage(Texture* img, float x, float y, int anc);

//otf, ttf
const char* getStringName();
void setStringName(const char* str);

float getStringSize();
void setStringSize(float size);

void getStringRGBA(float& r, float& g, float& b, float& a);
void setStringRGBA(float r, float g, float b, float a);

float getStringBorder();
void setStringBorder(float border);

void getStringBorderRGBA(float& r, float& g, float& b, float& a);
void setStringBorderRGBA(float r, float g, float b, float a);

void drawString(float x, float y, int anc, const char* szFormat, ...);

iRect rectOfString(const char* szFormat, ...);

