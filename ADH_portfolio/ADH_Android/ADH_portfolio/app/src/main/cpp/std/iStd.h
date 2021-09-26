#pragma once

#include "iType.h"

#include <string.h>
#include <math.h>

#include "iPoint.h"
#include "iSize.h"
#include "iRect.h"
#include "iColor.h"
#include "iString.h"
#include "iStrTex.h"
#include "iOpenGL.h"
#include "iFBO.h"
#include "iMatrix.h"
#include "iImage.h"
#include "iPopup.h"

#include "iFPS.h"

#include "iArray.h"
#include "iMatrix.h"

//#define DEV_WIDTH 640
//#define DEV_HEIGHT 480
#define DEV_WIDTH 1280
#define DEV_HEIGHT 720
#define RATE_MONITOR 0.3f

extern iSize devSize;
extern iRect viewport;

#define va_start_end(szText, szFormat)	\
va_list args; \
va_start(args, szFormat); \
vsnprintf(szText, sizeof(szText), szFormat, args); \
va_end(args)
//percentX2(szText); \

void percentX2(char* szText);

void appInitialize();

bool setZoom(iPoint point, float scale, float deltaZoom, float deltaDelay);
float updateZoom(float dt, iPoint& p);

extern int keyStat, keyDown;
void setKeyStat(iKeyStat stat, int c);
void setKeyDown(iKeyStat stat, int c);

#if (OS==OS_WINDOW)

bool* keyBuf;
int random();

#elif (OS==OS_ANDROID)

#endif // (OS==OS_WINDOW)

float _cos(float degree);
float _sin(float degree);

float clamp(float x, float minVal, float maxVal);
iPoint clamp(iPoint x, iPoint minval, iPoint maxVal);

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

//void drawRect(const iRect& rt, float radius = 0.0f);
void drawRect(iRect rt, float radius = 0.0f);
void drawRect(float x, float y, float width, float height, float radius = 0.0f);

//void fillRect(const iRect& rt, float radius = 0.0f);
void fillRect(iRect rt, float radius = 0.0f);
void fillRect(float x, float y, float width, float height, float radius = 0.0f);

uint32 nextPot(uint32 x);
void copyBmp2Rgba(uint32* src, int stride, int x, int y, int w, int h,
                  uint32* dst, int dw, int dh);
void copyBmp2RGBABgAlpha(uint32* src, int stride, int x, int y, int w, int h,
                         uint32* dst, int dw, int dh);
Texture* createImageWithRGBA(uint8* rgba, int width, int height);
Texture* createTexture(int width, int height, bool rgba32f=false);
#define CHECK_TEXTURE 1
#if CHECK_TEXTURE
extern int texture_num;
#endif

Texture** createImageAlphaDivide(int numX, int numY, const char* szFormat, ...);
Texture* createImageAlpha(const char* szFormat, ...);
Texture** createImageDivide(int numX, int numY, const char* szFormat, ...);
Texture* createImage(const char* szFormat, ...);
void freeImage(Texture* tex);
void drawImage(Texture* tex, float x, float y, int anc,
               float ix, float iy, float iw, float ih,
               float rx, float ry,
               int xyz, float degree, int reverse = REVERSE_NONE);
void drawImage(Texture* tex, float x, float y, int anc);

const char* getStringName();
void setStringName(const char* str);

void setStringSize(float size);
float getStringSize();

void setStringLineHeight(float height);
float getStringLineHeight();

void setStringRGBA(float r, float g, float b, float a);
void getStringRGBA(float& r, float& g, float& b, float& a);

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
