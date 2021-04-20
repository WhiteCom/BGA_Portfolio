#pragma once

#if 0
// Windows 헤더 파일
#include <windows.h>

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

#include "iType.h"


typedef void igImage;

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

public:


};

#endif