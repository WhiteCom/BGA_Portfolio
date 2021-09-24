#pragma once

#include "iType.h"

#include "iPoint.h"
#include "iString.h"
#include "iArray.h"
#include "iSize.h"
#include "iRect.h"
#include "iColor.h"
#include "iImage.h"
#include "iPopup.h"
#include "iButton.h"

#include "iSort.h"
#include "iFPS.h"
#include "iGraphics.h"

#include "iOpenGL.h"
#include "iOpenAL.h"
#include "iStrTex.h"
#include "iFBO.h"

#include "iMatrix.h"

#include "iNumber.h"

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

//for openAL
#define SOUND_NUM 7

//OS : Android / Window
#ifdef loge
#define xprint loge
#else
#define xprint printf
#endif

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

bool setZoom(iPoint point, float scale, float deltaZoom, float deltaDelay);
float updateZoom(float dt, iPoint& p);

int random();

float _cos(float degree);
float _sin(float degree);

float uint8ToFloat(uint8 c);
uint8 floatToUint8(float c);

void setRGBA(iColor4f c);
void setRGBA(iColor4b c);
void setRGBA(float r, float g, float b, float a);
void getRGBA(float& r, float& g, float& b, float& a);

void setLineWidth(float width);
float getLineWidth();

void clearRect();
void setClip(float x, float y, float width, float height);

void setDotSize(float size);
void drawDot(float x, float y);

void drawCircle(float x, float y, float radius);
void fillCircle(float x, float y, float radius);

void drawLine(iPoint sp, iPoint ep);
void drawLine(float x0, float y0, float x1, float y1);

void drawRect(iRect rt, float radius = 0.0f);
void drawRect(float x, float y, float width, float height, float radius = 0.0f);

void fillRect(iRect rt, float ridius = 0.0f);
void fillRect(float x, float y, float width, float height, float radius = 0.0f);

uint32 nextPot(uint32 x);
void copyBmp2RGBA(uint32* src, int stride, int x, int y, int w, int h,
	uint32* dst, int dw, int dh);
void copyBmp2RGBABgAlpha(uint32* src, int stride, int x, int y, int w, int h,
	uint32* dst, int dw, int dh);

Texture* createImageWithRGBA(uint8* rgba, int width, int height);
Texture* createTexture(int width, int height, bool rgba32F = false);
#define CHECK_TEXTURE 1
#if CHECK_TEXTURE
	extern int texture_num;
#endif

Texture** createImageDivide(int numX, int numY, const char* szFormat, ...);
Texture* createImage(const char* szFormat, ...);
Texture** createImageAlphaDivide(int numX, int numY, const char* szFormat, ...);
Texture* createImageAlpha(const char* szFormat, ...);
void freeImage(Texture* tex);
void drawImage(Texture* tex, float x, float y, int anc,
	float ix, float iy, float iw, float ih,
	float rx, float ry,
	int xyz, float degree, int reverse = REVERSE_NONE);
void drawImage(Texture* img, float x, float y, int anc);

//otf, ttf
const char* getStringName();
void setStringName(const char* str);

float getStringSize();
void setStringSize(float size);

void setStringLineHeight(float height);
float getStringLineHeight();

void getStringRGBA(float& r, float& g, float& b, float& a);
void setStringRGBA(float r, float g, float b, float a);

float getStringBorder();
void setStringBorder(float border);

void getStringBorderRGBA(float& r, float& g, float& b, float& a);
void setStringBorderRGBA(float r, float g, float b, float a);

struct iFont
{
	float height;
	float interval;
	Texture** texs;
};
iFont* loadFont(const char* strOTFTTF, float height, const char* strUse);
void freeFont(iFont* font);
void setFont(iFont* font);

void drawString(float x, float y, int anc, const char* szFormat, ...);
void drawString(float x, float y, float sx, float sy, int anc, const char* szFormat, ...);
iRect rectOfString(const char* szFormat, ...);

char* loadFile(const char* fileName, int& length);
void saveFile(const char* fileName, char* buf, int length);

