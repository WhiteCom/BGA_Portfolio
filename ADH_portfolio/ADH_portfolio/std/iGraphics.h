#pragma once

#include <windows.h>

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

#include "iType.h"
#include "iSize.h"
#include "iRect.h"

typedef Image igImage;

class iGraphics
{
private:
	iGraphics();
public:
	static iGraphics* share();
public:
	virtual ~iGraphics();

	//gdiplus
	void start(HDC hdc);
	void end();

	void init(iSize size);
	void init(int width, int height);

	void clear(float r, float g, float b, float a);
	void clear();

	Texture* getTexture();

	void drawLine(iPoint sp, iPoint ep);
	void drawLine(float x0, float y0, float x1, float y1);

	void drawRect(iRect rt, float radius = 0.0f);
	void drawRect(float x, float y, float width, float height, float radius = 0.0f);

	void fillRect(iRect rt, float radius = 0.0f);
	void fillRect(float x, float y, float width, float height, float radius = 0.0f);

	igImage* createIgImage(const char* szFormat, ...);
	void freeImage(igImage* tex);
	void drawigImage(igImage* tex, float x, float y, int anc,
		float ix, float iy, float iw, float ih,
		float rx, float ry,
		int xyz, float degree, int reverse = REVERSE_NONE);
	void drawigImage(igImage* img, float x, float y, int anc);

	static iRect rectOfString(const char* szFormat, ...);
	void drawString(float x, float y, int anc, const char* szFormat, ...);
public:
	Bitmap* bmp;
	Graphics* g;
};

wchar_t* utf8_to_utf16(const char* szFormat, ...);
char* utf16_to_utf8(const wchar_t* str);
